#include "../inc/symbol_json.hpp"
#include "../inc/converters.hpp"
#include <iostream>

SymbolJsonRecord::SymbolJsonRecord(Symbol *sym)
{
  if (sym)
    this->init_from_symbol(sym);
}

SymbolJsonRecord::SymbolJsonRecord(Section *sym)
{
  if (sym)
    this->init_from_section_symbol(sym);
}

SymbolJsonRecord::SymbolJsonRecord(std::string json_file)
{
  this->init_from_json(json_file);
}

void SymbolJsonRecord::set_value(int32_t val)
{
  this->value = val;
  this->has_value = true;
}

void SymbolJsonRecord::set_section(uint32_t section_id)
{
  this->section = section_id;
  this->has_section = true;
}

void SymbolJsonRecord::set_is_global(bool is_global)
{
  this->is_global = is_global;
}

void SymbolJsonRecord::set_is_final(bool is_final)
{
  this->is_final = is_final;
}

void SymbolJsonRecord::set_type(type::PARAMETER_TYPE type)
{
  this->type = type;
}

void SymbolJsonRecord::set_id(uint32_t id)
{
  this->id = id;
}

void SymbolJsonRecord::set_name(std::string name)
{
  this->name = name;
}

void SymbolJsonRecord::set_name(char *name)
{
  std::string str_name(name);
  this->set_name(str_name);
}

bool SymbolJsonRecord::get_value(int32_t *val)
{
  if (this->has_value)
    *val = this->value;
  return this->has_value;
}

bool SymbolJsonRecord::get_section(uint32_t *section_id)
{
  if (this->has_section)
    *section_id = this->section;
  return this->has_section;
}

bool SymbolJsonRecord::get_is_global()
{
  return this->is_global;
}

bool SymbolJsonRecord::get_is_final()
{
  return this->is_final;
}

type::PARAMETER_TYPE SymbolJsonRecord::get_type()
{
  return this->type;
}

uint32_t SymbolJsonRecord::get_id()
{
  return this->id;
}

std::string SymbolJsonRecord::get_name()
{
  return this->name;
}

const std::string NAME_KEY = "name:";
const std::string ID_KEY = "id:";
const std::string SECTION_KEY = "section:";
const std::string HAS_SECTION_KEY = "has_section:";
const std::string VALUE_KEY = "value:";
const std::string HAS_VALUE_KEY = "has_value:";
const std::string IS_GLOBAL_KEY = "is_global:";
const std::string IS_FINAL_KEY = "is_final:";
const std::string TYPE_KEY = "type:";

std::string SymbolJsonRecord::convert_to_json()
{
  std::string out_json = "{\n";
  out_json += (NAME_KEY + this->name + ",\n");
  out_json += (ID_KEY + std::to_string(this->id) + ",\n");
  out_json += (SECTION_KEY + std::to_string(this->section) + ",\n");
  out_json += (HAS_SECTION_KEY + std::to_string(this->has_section) + ",\n");
  out_json += (VALUE_KEY + std::to_string(this->value) + ",\n");
  out_json += (HAS_VALUE_KEY + std::to_string(this->has_value) + ",\n");
  out_json += (IS_GLOBAL_KEY + std::to_string(this->is_global) + ",\n");
  out_json += (IS_FINAL_KEY + std::to_string(this->is_final) + ",\n");
  out_json += (TYPE_KEY + std::to_string(static_cast<int>(this->type)) + ",\n");
  out_json += "}";

  // std::cout << out_json.c_str() << std::endl;
  return out_json;
}

void SymbolJsonRecord::init_from_json(std::string json_file)
{
  if (json_file[0] != '{' || json_file[json_file.size() - 1] != '}')
  {
    std::cout << "Wrong json format: " << json_file << std::endl;
    return;
  }
  uint32_t pos = 1;
  std::string val;

  val = converter::get_value_from_json(json_file, NAME_KEY, &pos);
  this->name = val;

  val = converter::get_value_from_json(json_file, ID_KEY, &pos);
  this->id = (uint32_t)std::stoul(val);

  val = converter::get_value_from_json(json_file, SECTION_KEY, &pos);
  this->section = (uint32_t)std::stoul(val);

  val = converter::get_value_from_json(json_file, HAS_SECTION_KEY, &pos);
  this->has_section = std::stoi(val);

  val = converter::get_value_from_json(json_file, VALUE_KEY, &pos);
  this->value = (int32_t)std::stoul(val);

  val = converter::get_value_from_json(json_file, HAS_VALUE_KEY, &pos);
  this->has_value = std::stoi(val);

  val = converter::get_value_from_json(json_file, IS_GLOBAL_KEY, &pos);
  this->is_global = std::stoi(val);

  val = converter::get_value_from_json(json_file, IS_FINAL_KEY, &pos);
  this->is_final = std::stoi(val);

  val = converter::get_value_from_json(json_file, TYPE_KEY, &pos);
  this->type = static_cast<type::PARAMETER_TYPE>(std::stoi(val));
}

void SymbolJsonRecord::init_from_symbol(Symbol *sym)
{
  this->name = sym->get_name();
  this->id = sym->get_id();
  if (sym->get_section())
    this->set_section(sym->get_section()->get_id());
  else
    this->has_section = false;
  this->value = sym->get_value();
  this->has_value = sym->has_set_value();
  this->is_global = sym->get_global_flag();
  this->is_final = sym->get_final_flag();
  this->type = sym->get_type(); // can be only SYMBOL
}

void SymbolJsonRecord::init_from_section_symbol(Section *sym)
{ // TODO: check
  this->name = sym->get_name();
  this->id = sym->get_id();
  this->has_section = false;
  this->set_value(sym->get_length());
  this->is_global = true;
  this->is_final = false;       // because if it's initialized with Section instance, can't be final (because it's still assembly stage)
  this->type = sym->get_type(); // can be only SECTION
}
