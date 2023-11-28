#include "../inc/assembler.hpp"
#include <iostream>
#include <cstring>

extern int yyparse();

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
  if (!this->is_running)
    return nullptr;

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

  return sym;
}

Symbol *Assembler::add_symbol(char *name)
{
  if (!this->is_running)
    return nullptr;

  std::string str_name(name);
  return this->add_symbol(str_name);
}

Symbol *Assembler::create_symbol(std::string name)
{
  if (!this->is_running)
    return nullptr;

  // if already exist, just return it from symbol table
  if (this->symbol_table.find(name) != this->symbol_table.end())
    return this->symbol_table[name];

  // if symbol doesn't exist create new one
  // and add it to symbol table
  Symbol *sym = new Symbol(name);
  this->set_id_to_sym(sym);
  this->symbol_table[name] = sym;
  return sym;
}

Symbol *Assembler::create_symbol(char *name)
{
  if (!this->is_running)
    return nullptr;

  std::string str_name(name);
  return this->create_symbol(str_name);
}

void Assembler::add_section(std::string section_name)
{
  if (!this->is_running)
    return;

  Section *new_section = nullptr;

  // create new section if it's not already existing
  if (this->section_table.find(section_name) == this->section_table.end())
  {
    new_section = new Section(section_name);

    // try to reuse the id (if the symbol which is used for defining the section doesn't exist for itself ==> is not defined)
    Symbol *sym = nullptr;
    if (this->symbol_table.find(section_name) != this->symbol_table.end())
      sym = this->symbol_table[section_name];

    if (!sym || sym->get_defined_flag())
    {
      this->set_id_to_sym(new_section); // create new id
    }
    else
    {
      new_section->set_id(sym->get_id());     // reuse id of non-defined symbol
      this->symbol_table.erase(section_name); // remove unused symbol
    }
    this->section_table[section_name] = new_section;
  }

  if (!new_section)
    new_section = this->section_table[section_name];

  this->curr_section = new_section;
}

void Assembler::add_command(Command *cmd)
{
  if (!this->is_running)
    return;

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

bool Assembler::get_running_flag() const
{
  return this->is_running;
}

void Assembler::stop()
{
  this->is_running = false;
}

bool Assembler::run()
{
  this->is_running = true;
  yyparse();

  std::cout << "finished parsing..." << std::endl;

  std::cout << "executing section: __NO_DATA_SECTION__" << std::endl;
  this->no_data_section->create_output_file();
  std::vector<type::byte> output_file = this->no_data_section->get_output_file();
  std::list<RelocationRecord *> realocations = this->no_data_section->get_all_relocations();

  // FIXME: (only for test purposes) from here...
  std::cout << "0x00: | ";
  type::byte new_line_cnt = 0;
  for (type::byte single_byte : output_file)
  {
    std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)single_byte << " | ";

    if (++new_line_cnt % 4 == 0)
    {
      std::cout << std::endl;

      if (new_line_cnt < output_file.size())
        std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)new_line_cnt << ": | ";
    }
  }

  std::cout << std::endl
            << "Relocations: " << std::endl;
  for (RelocationRecord *rel : realocations)
  {
    std::cout << "Offset: " << rel->get_offset()
              << " | Addend: " << rel->get_addend()
              << " | Sym_id: " << rel->get_symbol_id()
              << " | Addend sign: " << rel->get_addend_signed_flag()
              << std::endl;
  }
  // FIXME: ...until here

  for (auto &iter : this->section_table)
  {
    Section *section = iter.second;
    std::cout << std::endl
              << "executing section: " << iter.first << std::endl;
    section->create_output_file();
    std::vector<type::byte> output_file = section->get_output_file();
    std::list<RelocationRecord *> realocations = section->get_all_relocations();

    // FIXME: (only for test purposes) from here...
    std::cout << "0x00: | ";
    new_line_cnt = 0;
    for (type::byte single_byte : output_file)
    {
      std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)single_byte << " | ";

      if (++new_line_cnt % 4 == 0)
      {
        std::cout << std::endl;

        if (new_line_cnt < output_file.size())
          std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)new_line_cnt << ": | ";
      }
    }

    std::cout << std::endl
              << "Relocations: " << std::endl;
    for (RelocationRecord *rel : realocations)
    {
      std::cout << "Offset: " << rel->get_offset()
                << " | Addend: " << rel->get_addend()
                << " | Sym_id: " << rel->get_symbol_id()
                << " | Addend sign: " << rel->get_addend_signed_flag()
                << std::endl;
    }
    // FIXME: ...until here
  }

  std::cout << std::endl
            << "SYMBOL TABLE: " << std::endl;

  char *s = new char[100];
  sprintf(s, "%10s | %2s | %10s | %7s | %8s | %7s |", "NAME", "ID", "SECTION", "GLOBAL", "VALUE", "TYPE");
  std::cout << s << std::endl;
  uint8_t padding = 6; // %10s - strlen("NAME")
  for (uint8_t i = 0; i < strlen(s) + padding; i++)
    std::cout << "-";
  std::cout << std::endl;

  for (auto &iter : this->symbol_table)
  {
    Symbol *sym = iter.second;
    if (!sym->get_defined_flag())
      continue;

    s = new char[100];
    sprintf(s, "%10s | %2d | %10s | %7s | %8s | %7s |", sym->get_name().c_str(), sym->get_id(), sym->get_section() != nullptr ? std::to_string(sym->get_section()->get_id()).c_str() : "NO_SECTION", sym->get_global_flag() ? "true" : "false", sym->has_set_value() ? std::to_string(sym->get_value()).c_str() : "NO_VALUE", "SYMBOL");
    std::cout << s << std::endl;
  }

  for (auto &iter : this->section_table)
  {
    Section *sym = iter.second;

    s = new char[100];
    sprintf(s, "%10s | %2d | %10s | %7s | %8s | %7s |", sym->get_name().c_str(), sym->get_id(), "", "true", std::to_string(sym->get_length()).c_str(), "SECTION");
    std::cout << s << std::endl;
  }

  return this->is_running;
}