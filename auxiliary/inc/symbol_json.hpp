#ifndef SYMBOL_JSON_RECORD_HPP
#define SYMBOL_JSON_RECORD_HPP

#include "../../inc/types.hpp"
#include "../../inc/symbol.hpp"
#include "../../inc/section.hpp"
#include <string>

class SymbolJsonRecord
{
private:
  std::string name;
  uint32_t id;
  uint32_t section;
  bool has_section = false;
  int32_t value;
  bool has_value = false;
  bool is_global = false;
  bool is_final = false;
  type::PARAMETER_TYPE type;

public:
  SymbolJsonRecord() {}

  void set_value(int32_t val);           // sets value and has_value=true
  void set_section(uint32_t section_id); // sets section id and has_section=true
  void set_is_global(bool is_global);
  void set_is_final(bool is_final);
  void set_type(type::PARAMETER_TYPE type);
  void set_id(uint32_t id);
  void set_name(std::string name);
  void set_name(char *name);

  bool get_value(int32_t *val);           // nullptr if has_value==false
  bool get_section(uint32_t *section_id); // nullptr if has_section==false
  bool get_is_global();
  bool get_is_final();
  type::PARAMETER_TYPE get_type();
  uint32_t get_id();
  std::string get_name();

  std::string convert_to_json();
  void init_from_json(std::string json_file);
  void init_from_symbol(Symbol *sym);
  void init_from_section_symbol(Section *sym);
};

#endif