#ifndef OBJECT_FILE_HPP
#define OBJECT_FILE_HPP

#include "../../inc/types.hpp"
#include "../../inc/section.hpp"
#include "../../inc/symbol.hpp"
#include "section_json.hpp"
#include "symbol_json.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class ObjectFile
{
private:
  std::vector<SectionJsonRecord> sections;
  std::vector<SymbolJsonRecord> sym_table;

  // <section_name, index> pairs
  std::unordered_map<std::string, uint32_t> section_lookup_map;

  // <symbol_id, index> pairs
  std::unordered_map<uint32_t, uint32_t> symbol_lookup_map;

public:
  ObjectFile() = default;
  ObjectFile(std::string json_file);
  ObjectFile(std::vector<SectionJsonRecord> sections, std::vector<SymbolJsonRecord> sym_table);
  ObjectFile(std::vector<SectionJsonRecord> sections);
  ObjectFile(std::vector<SymbolJsonRecord> sym_table);
  ObjectFile(std::vector<Section *> sections, std::vector<Parameter *> sym_table);
  ObjectFile(std::vector<Section *> sections);
  ObjectFile(std::vector<Parameter *> sym_table);

  void set_sections(std::vector<SectionJsonRecord> sections);
  void set_symbol_table(std::vector<SymbolJsonRecord> sym_table);
  void set_sections(std::vector<Section *> sections);
  void set_symbol_table(std::vector<Parameter *> sym_table); // Parameter can be either SYMBOL or SECTION

  void add_section(SectionJsonRecord section);
  void add_symbol(SymbolJsonRecord symbol);
  void add_section(Section *section);
  void add_symbol(Parameter *symbol); // Parameter can be either SYMBOL or SECTION

  std::vector<SectionJsonRecord> get_sections();
  std::vector<SymbolJsonRecord> get_symbol_table();

  std::string convert_to_json();
  void init_from_json(std::string json_file);
  bool create_lookup_maps();
  SectionJsonRecord *get_section(std::string section_name);
  SymbolJsonRecord *get_symbol(uint32_t symbol_id);
};

#endif