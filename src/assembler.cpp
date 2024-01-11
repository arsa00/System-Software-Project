#include "../inc/assembler.hpp"
#include "../auxiliary/inc/converters.hpp"
#include <iostream>
#include <cstring>
#include <fstream>
#include "../auxiliary/inc/symbol_json.hpp"
#include "../auxiliary/inc/section_json.hpp"
#include "../auxiliary/inc/object_file.hpp"

extern int yyparse();

Assembler &Assembler::get_instance()
{
  static Assembler assembler_instance;
  return assembler_instance;
}

Assembler::Assembler()
{
  std::cout << "[ASSEMBLER]: Created Assembler instance." << std::endl;
  // this->no_data_section = new Section("__NO_DATA_SECTION__");

  // set .text to be default section
  Section *default_section = new Section(".text");
  this->set_id_to_sym(default_section); // create new id
  this->section_table[".text"] = default_section;
  this->curr_section = default_section;
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

  // if (this->no_data_section)
  //   delete this->no_data_section;
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

void Assembler::set_output_file_name(std::string file_name)
{
  this->output_file_name = file_name;
}

std::string Assembler::get_output_file_name()
{
  return this->output_file_name;
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
      delete sym;
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
  // if (!this->curr_section && !cmd->get_generate_data_flag())
  // {
  //   this->no_data_section->add_command(cmd);
  //   return;
  // }

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
  if (this->output_file_name.empty())
  {
    this->internal_error("Output file must be set.");
    return false;
  }

  this->is_running = true;
  yyparse();

  std::cout << "finished parsing..." << std::endl;

  // execute __NO_DATA_SECTION__, although it should be empty
  // std::cout << "executing section: __NO_DATA_SECTION__" << std::endl;
  // this->no_data_section->create_output_file();
  // std::vector<type::byte> output_file = this->no_data_section->get_output_file();
  // std::list<RelocationRecord *> relocations = this->no_data_section->get_all_relocations();

  // if (output_file.size() > 0 || relocations.size() > 0)
  // {
  //   this->internal_error("Something written to __NO_DATA_SECTION__");
  // }

  // create output obj file and add all sections to it (after executing sections!!!)
  ObjectFile *out_obj_file = new ObjectFile();
  // out_obj_file->add_section(this->no_data_section);

  for (auto &iter : this->section_table)
  {
    Section *section = iter.second;
    std::cout << std::endl
              << "executing section: " << iter.first << std::endl;
    section->create_output_file();
    out_obj_file->add_section(section);
    out_obj_file->add_symbol(section);
  }

  if (this->internal_err || this->parsing_err)
  {
    this->is_running = false;
    return false; // failed
  }

  // add all symbols to output obj file
  for (auto &iter : this->symbol_table)
  {
    Symbol *sym = iter.second;
    if (!sym->get_defined_flag())
      continue;

    out_obj_file->add_symbol(sym);
  }

  // print sections
  std::cout << std::endl;

  char *s; // s is used for formatted output
  for (auto &iter : this->section_table)
  {
    Section *section = iter.second;
    std::cout << std::endl
              << "SECTION: " << iter.first << std::endl;
    std::vector<type::byte> output_file = section->get_output_file();
    std::list<RelocationRecord *> relocations = section->get_all_relocations();

    std::cout << "0x00: | ";
    uint32_t new_line_cnt = 0;
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
              << "RELOCATIONS: " << std::endl;

    s = new char[100];
    sprintf(s, "%10s | %7s | %7s | %8s | %7s", "OFFSET", "SYM_ID", "ADDEND", "SIGN", "TYPE");
    std::cout << s << std::endl;

    for (uint8_t i = 0; i < strlen(s) + 4; i++)
      std::cout << "-";
    std::cout << std::endl;
    for (RelocationRecord *rel : relocations)
    {
      s = new char[100];
      sprintf(s, "%#010x | %7d | %7d | %8s | %7s", rel->get_offset(), rel->get_symbol_id(), rel->get_addend(), rel->get_addend_signed_flag() ? "SIGNED" : "UNSIGNED", converter::relocation_type_to_string(rel->get_type()).c_str());
      std::cout << s << std::endl;
    }
  }

  // print symbol table
  std::cout << std::endl
            << "SYMBOL TABLE: " << std::endl;

  s = new char[100];
  sprintf(s, "%15s | %3s | %10s | %7s | %10s | %7s |", "NAME", "ID", "SECTION", "GLOBAL", "VALUE", "TYPE");
  std::cout << s << std::endl;
  uint8_t padding = 11; // %15s - strlen("NAME")
  for (uint8_t i = 0; i < strlen(s) + padding; i++)
    std::cout << "-";
  std::cout << std::endl;

  for (auto &iter : this->symbol_table)
  {
    Symbol *sym = iter.second;
    if (!sym->get_defined_flag())
      continue;

    s = new char[100];
    char *value_str = new char[12];
    if (sym->has_set_value())
    {
      sprintf(value_str, "%#010x", sym->get_value());
    }
    else
    {
      value_str = "NO_VALUE";
    }
    sprintf(s, "%15s | %3d | %10s | %7s | %10s | %7s |", sym->get_name().c_str(), sym->get_id(), sym->get_section() != nullptr ? std::to_string(sym->get_section()->get_id()).c_str() : "NO_SECTION", sym->get_global_flag() ? "true" : "false", value_str, "SYMBOL");
    std::cout << s << std::endl;
  }

  for (auto &iter : this->section_table)
  {
    Section *sym = iter.second;

    s = new char[100];
    char *value_str = new char[12];
    sprintf(value_str, "%#010x", sym->get_length());
    sprintf(s, "%15s | %3d | %10s | %7s | %10s | %7s |", sym->get_name().c_str(), sym->get_id(), "", "true", value_str, "SECTION");
    std::cout << s << std::endl;
  }

  // get json content from output obj file
  std::string output_json_content = out_obj_file->convert_to_json();

  // create output file
  std::ofstream out_file(this->output_file_name);

  // write to output file
  if (out_file)
  {
    out_file << output_json_content;
  }
  else
  {
    // something went wrong
    this->internal_error("Cannot write to output assembler file");
    this->is_running = false;
    return false;
  }

  // close output file
  out_file.close();

  // assembler successfully finished
  this->is_running = false;
  return true; // success
}