#include "../inc/section_json.hpp"
#include "../inc/converters.hpp"
#include <iostream>

// Relocation json record
RelocationJsonRecord::RelocationJsonRecord(RelocationRecord *rel_record)
{
  if (rel_record)
  {
    this->init_from_relocation(rel_record);
  }
}

RelocationJsonRecord::RelocationJsonRecord(std::string json_file)
{
  this->init_from_json(json_file);
}

void RelocationJsonRecord::set_offset(uint32_t offset)
{
  this->offset = offset;
}

void RelocationJsonRecord::set_addend(uint32_t addend)
{
  this->addend = addend;
}

void RelocationJsonRecord::set_is_addend_signed(bool is_addend_signed)
{
  this->is_addend_signed = is_addend_signed;
}

void RelocationJsonRecord::set_type(type::RELOCATIONS type)
{
  this->type = type;
}

void RelocationJsonRecord::set_sym_id(uint32_t id)
{
  this->sym_id = id;
}

uint32_t RelocationJsonRecord::get_offset()
{
  return this->offset;
}

uint32_t RelocationJsonRecord::get_addend()
{
  return this->addend;
}

bool RelocationJsonRecord::get_is_addend_signed()
{
  return this->is_addend_signed;
}

type::RELOCATIONS RelocationJsonRecord::get_type()
{
  return this->type;
}

uint32_t RelocationJsonRecord::get_sym_id()
{
  return this->sym_id;
}

const std::string OFFSET_KEY = "offset:";
const std::string ADDEND_KEY = "addend:";
const std::string IS_ADDEND_SIGNED_KEY = "is_addend_signed:";
const std::string SYM_ID_KEY = "sym_id:";
const std::string TYPE_KEY = "type:";

std::string RelocationJsonRecord::convert_to_json()
{
  std::string out_json = "{\n";
  out_json += (OFFSET_KEY + std::to_string(this->offset) + ",\n");
  out_json += (ADDEND_KEY + std::to_string(this->addend) + ",\n");
  out_json += (IS_ADDEND_SIGNED_KEY + std::to_string(this->is_addend_signed) + ",\n");
  out_json += (SYM_ID_KEY + std::to_string(this->sym_id) + ",\n");
  out_json += (TYPE_KEY + std::to_string(static_cast<int>(this->type)) + ",\n");
  out_json += "}";

  // std::cout << out_json.c_str() << std::endl;
  return out_json;
}

void RelocationJsonRecord::init_from_json(std::string json_file)
{
  if (json_file[0] != '{' || json_file[json_file.size() - 1] != '}')
  {
    std::cout << "Wrong json format: " << json_file << std::endl;
    return;
  }
  uint32_t pos = 1;
  std::string val;

  val = converter::get_value_from_json(json_file, OFFSET_KEY, &pos);
  this->offset = (uint32_t)std::stoul(val);

  val = converter::get_value_from_json(json_file, ADDEND_KEY, &pos);
  this->addend = (uint32_t)std::stoul(val);

  val = converter::get_value_from_json(json_file, IS_ADDEND_SIGNED_KEY, &pos);
  this->is_addend_signed = std::stoi(val);

  val = converter::get_value_from_json(json_file, SYM_ID_KEY, &pos);
  this->sym_id = (uint32_t)std::stoul(val);

  val = converter::get_value_from_json(json_file, TYPE_KEY, &pos);
  this->type = static_cast<type::RELOCATIONS>(std::stoi(val));
}

void RelocationJsonRecord::init_from_relocation(RelocationRecord *rel_record)
{
  if (!rel_record)
    return;

  this->offset = rel_record->get_offset();
  this->addend = rel_record->get_addend();
  this->is_addend_signed = rel_record->get_addend_signed_flag();
  this->sym_id = rel_record->get_symbol_id();
  this->type = rel_record->get_type();
}

// Section json record
SectionJsonRecord::SectionJsonRecord(Section *section)
{
  if (section)
  {
    this->init_from_section(section);
  }
}

SectionJsonRecord::SectionJsonRecord(std::string json_file)
{
  this->init_from_json(json_file);
}

void SectionJsonRecord::set_output_file(std::vector<type::byte> output_file)
{
  this->output_file = output_file;
}

void SectionJsonRecord::set_relocations(std::vector<RelocationJsonRecord> relocations)
{
  this->relocations = relocations;
}

void SectionJsonRecord::set_id(uint32_t id)
{
  this->id = id;
}

void SectionJsonRecord::add_to_output_file(type::byte single_byte)
{
  this->output_file.push_back(single_byte);
}

void SectionJsonRecord::add_relocation(RelocationJsonRecord json_rel_record)
{
  this->relocations.push_back(json_rel_record);
}

std::vector<type::byte> SectionJsonRecord::get_output_file()
{
  return this->output_file;
}

std::vector<RelocationJsonRecord> SectionJsonRecord::get_relocations()
{
  return this->relocations;
}

uint32_t SectionJsonRecord::get_id()
{
  return this->id;
}

const std::string OUTPUT_FILE_KEY = "out_hex_file:";
const std::string RELOCATIONS_KEY = "relocations:";
const std::string ID_KEY = "id:";

std::string SectionJsonRecord::convert_to_json()
{
  std::string out_json = "{\n";

  out_json += (ID_KEY + std::to_string(this->id) + ",\n");

  out_json += (OUTPUT_FILE_KEY + "[");
  for (uint32_t i = 0; i < this->output_file.size(); i++)
  {
    type::byte single_byte = this->output_file[i];
    out_json += std::to_string(single_byte);
    if (i != this->output_file.size() - 1)
      out_json += ",";
  }
  out_json += "],\n";

  out_json += (RELOCATIONS_KEY + "[");
  for (uint32_t i = 0; i < this->relocations.size(); i++)
  {
    RelocationJsonRecord json_rel_record = this->relocations[i];
    out_json += json_rel_record.convert_to_json();
    if (i != this->relocations.size() - 1)
      out_json += ",";
  }
  out_json += "],\n";

  out_json += "}";

  // std::cout << out_json.c_str() << std::endl;
  return out_json;
}

void SectionJsonRecord::init_from_json(std::string json_file)
{
  if (json_file[0] != '{' || json_file[json_file.size() - 1] != '}')
  {
    std::cout << "Wrong json format: " << json_file << std::endl;
    return;
  }
  uint32_t pos = 1;
  std::string val;

  // reset current values
  this->output_file = {};
  this->relocations = {};

  val = converter::get_value_from_json(json_file, ID_KEY, &pos);
  this->id = (uint32_t)std::stoul(val);

  val = converter::get_value_from_json(json_file, OUTPUT_FILE_KEY, &pos, true);
  std::vector<std::string> output_file_str = converter::decode_json_array(val);

  for (std::string str : output_file_str)
  {
    if (str.empty())
      continue;

    this->add_to_output_file(static_cast<type::byte>(std::stoul(str)));
  }

  val = converter::get_value_from_json(json_file, RELOCATIONS_KEY, &pos, true);
  std::vector<std::string> relocations_str = converter::decode_json_array(val);

  for (std::string str : relocations_str)
  {
    if (str.empty())
      continue;

    this->add_relocation(RelocationJsonRecord(str));
  }
}

void SectionJsonRecord::init_from_section(Section *section)
{
  this->id = section->get_id();
  this->output_file = section->get_output_file();
  std::list<RelocationRecord *> rel_records = section->get_all_relocations();
  for (RelocationRecord *rel_record : rel_records)
  {
    this->add_relocation(RelocationJsonRecord(rel_record));
  }
}
