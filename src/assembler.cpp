#include "../inc/assembler.hpp"
#include <iostream>

Assembler &Assembler::get_instance()
{
  static Assembler assembler_instance;
  return assembler_instance;
}

Assembler::Assembler()
{
  std::cout << "[ASSEMBLER]: Created Assembler instance." << std::endl;
  this->no_data_section = new Section("__NO_DATA_SECTION__");
}

Assembler::~Assembler()
{
  while (!this->symbol_table.empty())
  {
    auto iter_begin = this->symbol_table.begin();
    this->symbol_table.erase(iter_begin);
    delete iter_begin->second;
  }

  while (!this->section_table.empty())
  {
    auto iter_begin = this->section_table.begin();
    this->section_table.erase(iter_begin);
    delete iter_begin->second;
  }

  delete this->no_data_section;
}

void Assembler::parse_error(std::string err_msg)
{
  // TODO: call yyerror function...
  this->parsing_err = true;
  std::cerr << "[ASSEMBLER]: { PARSING ERROR: " << err_msg << " }" << std::endl;
}

void Assembler::internal_error(std::string err_msg)
{ // TODO: add some panic exit of Assembler...
  this->internal_err = true;
  std::cerr << "[ASSEMBLER]: { INTERNAL ERROR: " << err_msg << " }" << std::endl;
}

void Assembler::set_id_to_sym(Parameter *param)
{
  bool res = param->set_id(this->symbol_id);
  if (res)
    this->symbol_id++;
}

Symbol *Assembler::add_symbol(std::string name)
{
  if (!this->curr_section)
  {
    this->parse_error("No section opened. Cannot define symbol: " + name);
    return nullptr;
  }

  Symbol *sym = nullptr;

  // check if symbol with given name is already present in symbol table
  if (this->symbol_table.find(name) != this->symbol_table.end())
    sym = this->symbol_table[name];
  else
    sym = this->create_symbol(name);

  // panic
  if (!sym)
  {
    this->internal_error("Error defining symbol: " + name);
    return nullptr;
  }

  // define symbol
  sym->set_section(this->curr_section);
  sym->set_value(this->curr_section->get_curr_loc_cnt());
  sym->set_defined_flag(true);
  this->set_id_to_sym(sym);

  return sym;
}

Symbol *Assembler::add_symbol(char *name)
{
  std::string str_name(name);
  return this->add_symbol(str_name);
}

Symbol *Assembler::create_symbol(std::string name)
{
  // if already exist, just return it from symbol table
  if (this->symbol_table.find(name) != this->symbol_table.end())
    return this->symbol_table[name];

  // if symbol doesn't exist create new one
  // and add it to symbol table
  Symbol *sym = new Symbol(name);
  this->symbol_table[name] = sym;
  return sym;
}

Symbol *Assembler::create_symbol(char *name)
{
  std::string str_name(name);
  return this->create_symbol(str_name);
}

void Assembler::add_section(std::string section_name)
{
  Section *new_section = nullptr;

  // create new section if it's not already existing
  if (this->section_table.find(section_name) == this->section_table.end())
  {
    new_section = new Section(section_name);
    this->set_id_to_sym(new_section);
    this->section_table[section_name] = new_section;
    std::cout << "[ASSEMBLER]: "
              << "created section with name: " << new_section->get_name() << std::endl;
  }

  if (!new_section)
    new_section = this->section_table[section_name];

  this->curr_section = new_section;

  std::cout << "[ASSEMBLER]: finished " << this->section_table[section_name]->get_name() << std::endl;
}

void Assembler::add_command(Command *cmd)
{
  // if there is no active section and command doesn't emit any data
  // add it to the __NO_DATA_SECTION__
  if (!this->curr_section && !cmd->get_generate_data_flag())
  {
    this->no_data_section->add_command(cmd);
    return;
  }

  if (!this->curr_section)
  {
    this->parse_error("No section opened. Cannot add command.");
    return;
  }

  this->curr_section->add_command(cmd);
}