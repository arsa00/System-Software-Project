#include "../inc/assembler.hpp"
#include <iostream>

Assembler &Assembler::get_instance()
{
  static Assembler assembler_instance;
  return assembler_instance;
}

Assembler::~Assembler()
{
  while (!this->symbol_table.empty())
  {
    auto iter_begin = this->symbol_table.begin();
    this->symbol_table.erase(iter_begin);
    delete iter_begin->second;
  }
}

void Assembler::parse_error(std::string err_msg)
{
  // TODO: call yyerror function...
  this->parsing_error = true;
  std::cerr << "PARSING ERROR: " << err_msg << std::endl;
}

void Assembler::add_symbol(Symbol *sym)
{
  if (!this->curr_section)
  {
    this->parse_error("No section opened");
    return;
  }

  // check if symbol with given name is already present
  if (this->symbol_table.find(sym->get_name()) != this->symbol_table.end())
    return;

  sym->set_section(this->curr_section);
  symbol_table[sym->get_name()] = sym;
}

void Assembler::add_section(std::string section_name)
{
  // check if that section name isn't already used as symbol name // XXX: maybe this isn't needed at all?
  if (this->symbol_table.find(section_name) != this->symbol_table.end())
  {
    this->parse_error("Section name is already used by some symbol");
    return;
  }

  Section *new_section = nullptr;

  // create new section if it's not already existing
  if (this->section_table.find(section_name) == this->section_table.end())
  {
    new_section = new Section(section_name);
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
  if (!this->curr_section)
  {
    this->parse_error("No section opened");
    return;
  }

  this->curr_section->add_command(cmd);
}