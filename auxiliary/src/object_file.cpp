#include "../inc/object_file.hpp"
#include "../inc/converters.hpp"
#include <iostream>

ObjectFile::ObjectFile(std::string json_file)
{
  this->init_from_json(json_file);
}

ObjectFile::ObjectFile(std::vector<SectionJsonRecord> sections, std::vector<SymbolJsonRecord> sym_table)
{
  if (sections.size() > 0)
    this->set_sections(sections);
  if (sym_table.size() > 0)
    this->set_symbol_table(sym_table);
}

ObjectFile::ObjectFile(std::vector<SectionJsonRecord> sections)
{
  if (sections.size() > 0)
    this->set_sections(sections);
}

ObjectFile::ObjectFile(std::vector<SymbolJsonRecord> sym_table)
{
  if (sym_table.size() > 0)
    this->set_symbol_table(sym_table);
}

ObjectFile::ObjectFile(std::vector<Section *> sections, std::vector<Parameter *> sym_table)
{
  if (sections.size() > 0)
    this->set_sections(sections);
  if (sym_table.size() > 0)
    this->set_symbol_table(sym_table);
}

ObjectFile::ObjectFile(std::vector<Section *> sections)
{
  if (sections.size() > 0)
    this->set_sections(sections);
}

ObjectFile::ObjectFile(std::vector<Parameter *> sym_table)
{
  if (sym_table.size() > 0)
    this->set_symbol_table(sym_table);
}

void ObjectFile::set_sections(std::vector<SectionJsonRecord> sections)
{
  this->sections = sections;
}

void ObjectFile::set_symbol_table(std::vector<SymbolJsonRecord> sym_table)
{
  this->sym_table = sym_table;
}

void ObjectFile::set_sections(std::vector<Section *> sections)
{
  this->sections = {};
  for (Section *section : sections)
  {
    this->add_section(section);
  }
}

void ObjectFile::set_symbol_table(std::vector<Parameter *> sym_table)
{
  this->sym_table = {};
  for (Parameter *symbol : sym_table)
  {
    this->add_symbol(symbol);
  }
}

void ObjectFile::add_section(SectionJsonRecord section)
{
  this->sections.push_back(section);
}

void ObjectFile::add_symbol(SymbolJsonRecord symbol)
{
  this->sym_table.push_back(symbol);
}

void ObjectFile::add_section(Section *section)
{
  this->add_section(SectionJsonRecord(section));
}

void ObjectFile::add_symbol(Parameter *symbol)
{
  if (symbol->get_type() == type::PARAMETER_TYPE::SYMBOL)
  {
    this->sym_table.push_back(SymbolJsonRecord((Symbol *)symbol));
  }
  else if (symbol->get_type() == type::PARAMETER_TYPE::SECTION)
  {
    this->sym_table.push_back(SymbolJsonRecord((Section *)symbol));
  }
}

std::vector<SectionJsonRecord> ObjectFile::get_sections()
{
  return this->sections;
}

std::vector<SymbolJsonRecord> ObjectFile::get_symbol_table()
{
  return this->sym_table;
}

const std::string SECTIONS_KEY = "sections:";
const std::string SYMBOL_TABLE_KEY = "symbol_table:";

std::string ObjectFile::convert_to_json()
{
  std::string out_json = "{\n";

  out_json += (SECTIONS_KEY + "[");
  for (uint32_t i = 0; i < this->sections.size(); i++)
  {
    SectionJsonRecord json_section_record = this->sections[i];
    out_json += json_section_record.convert_to_json();
    if (i != this->sections.size() - 1)
      out_json += ",";
  }
  out_json += "],\n";

  out_json += (SYMBOL_TABLE_KEY + "[");
  for (uint32_t i = 0; i < this->sym_table.size(); i++)
  {
    SymbolJsonRecord json_sym_record = this->sym_table[i];
    out_json += json_sym_record.convert_to_json();
    if (i != this->sym_table.size() - 1)
      out_json += ",";
  }
  out_json += "],\n";

  out_json += "}";

  // std::cout << out_json.c_str() << std::endl;
  return out_json;
}

void ObjectFile::init_from_json(std::string json_file)
{
  if (json_file[0] != '{' || json_file[json_file.size() - 1] != '}')
  {
    std::cout << "Wrong json format: " << json_file << std::endl;
    return;
  }
  uint32_t pos = 1;
  std::string val;

  // reset current values
  this->sections = {};
  this->sym_table = {};

  val = converter::get_value_from_json(json_file, SECTIONS_KEY, &pos, true);
  std::vector<std::string> sections_str = converter::decode_json_array(val);

  for (std::string str : sections_str)
  {
    if (str.empty())
      continue;

    this->add_section(SectionJsonRecord(str));
  }

  val = converter::get_value_from_json(json_file, SYMBOL_TABLE_KEY, &pos, true);
  std::vector<std::string> sym_table_str = converter::decode_json_array(val);

  for (std::string str : sym_table_str)
  {
    if (str.empty())
      continue;

    this->add_symbol(SymbolJsonRecord(str));
  }
}

bool ObjectFile::create_lookup_maps()
{
  if (this->sym_table.size() == 0 || this->sections.size() == 0)
    return false;

  for (uint32_t i = 0; i < this->sym_table.size(); i++)
  {
    this->symbol_lookup_map[this->sym_table[i].get_id()] = i;
  }

  for (uint32_t i = 0; i < this->sections.size(); i++)
  {
    SymbolJsonRecord *section_sym = this->get_symbol(this->sections[i].get_id());
    std::string section_name = section_sym->get_name();
    this->section_lookup_map[section_name] = i;
  }

  return true;
}

SectionJsonRecord *ObjectFile::get_section(std::string section_name)
{
  if (this->section_lookup_map.size() == 0)
  {
    bool res = this->create_lookup_maps();
    if (!res)
      return nullptr;
  }

  // section name not present in sections list of obj file
  if (this->section_lookup_map.find(section_name) == this->section_lookup_map.end())
    return nullptr;

  // check if index is in range
  uint32_t section_index = this->section_lookup_map[section_name];
  if (section_index >= this->sections.size())
    return nullptr;

  // return section in O(1)
  return &this->sections[section_index];
}

SymbolJsonRecord *ObjectFile::get_symbol(uint32_t symbol_id)
{
  if (this->symbol_lookup_map.size() == 0)
  {
    bool res = this->create_lookup_maps();
    if (!res)
      return nullptr;
  }

  // symbol id not present in symbols list of obj file
  if (this->symbol_lookup_map.find(symbol_id) == this->symbol_lookup_map.end())
    return nullptr;

  // check if index is in range
  uint32_t symbol_index = this->symbol_lookup_map[symbol_id];
  if (symbol_index >= this->sym_table.size())
    return nullptr;

  // return symbol in O(1)
  return &this->sym_table[symbol_index];
}