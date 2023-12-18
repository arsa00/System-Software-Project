#ifndef SECTION_JSON_RECORD_HPP
#define SECTION_JSON_RECORD_HPP

#include "../../inc/types.hpp"
#include "../../inc/symbol.hpp"
#include "../../inc/section.hpp"
#include "../../inc/relocation_record.hpp"
#include <string>
#include <vector>

class RelocationJsonRecord
{
private:
  uint32_t offset;
  uint32_t addend;
  bool is_addend_signed;
  uint32_t sym_id;
  type::RELOCATIONS type;

public:
  RelocationJsonRecord() = default;
  RelocationJsonRecord(RelocationRecord *rel_record);
  RelocationJsonRecord(std::string json_file);

  void set_offset(uint32_t offset);
  void set_addend(uint32_t addend);
  void set_is_addend_signed(bool is_addend_signed);
  void set_type(type::RELOCATIONS type);
  void set_sym_id(uint32_t id);

  uint32_t get_offset();
  uint32_t get_addend();
  bool get_is_addend_signed();
  type::RELOCATIONS get_type();
  uint32_t get_sym_id();

  std::string convert_to_json();
  void init_from_json(std::string json_file);
  void init_from_relocation(RelocationRecord *rel_record);
};

class SectionJsonRecord
{
private:
  std::vector<type::byte> output_file;
  std::vector<RelocationJsonRecord> relocations;

  uint32_t id;
  uint32_t start_mem_addr = 0;
  bool is_section_placed = false;

public:
  SectionJsonRecord() = default;
  SectionJsonRecord(Section *section);
  SectionJsonRecord(std::string json_file);

  void set_output_file(std::vector<type::byte> output_file);
  void set_relocations(std::vector<RelocationJsonRecord> relocations);
  void set_id(uint32_t id);
  void set_start_mem_addr(uint32_t mem_addr);

  void add_to_output_file(type::byte single_byte);
  void add_relocation(RelocationJsonRecord json_rel_record);

  std::vector<type::byte> get_output_file();
  std::vector<RelocationJsonRecord> get_relocations();
  uint32_t get_id();
  uint32_t get_start_mem_addr();
  bool get_is_section_placed();

  std::string convert_to_json();
  void init_from_json(std::string json_file);
  void init_from_section(Section *section);
};

#endif