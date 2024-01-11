#ifndef LINKER_HPP
#define LINKER_HPP

#include "../auxiliary/inc/object_file.hpp"
#include <string>
#include <unordered_map>
#include <list>
#include <vector>
#include <functional>
#include <iomanip>

class Interval
{
private:
  uint32_t start = 0;
  uint32_t end = 0;

public:
  Interval(uint32_t start = 0, uint32_t end = 0) : start(start), end(end) {}

  void set_start(uint32_t start);
  void set_end(uint32_t end);

  uint32_t get_start();
  uint32_t get_end();
  std::string get_interval_string();

  bool is_value_in_interval(uint32_t val);
  bool is_interval_valid();
};

struct SymbolTableKey
{
  std::string sym_name;
  type::PARAMETER_TYPE sym_type;

  SymbolTableKey(std::string sym_name = "", type::PARAMETER_TYPE type = type::PARAMETER_TYPE::SYMBOL) : sym_name(sym_name), sym_type(type) {}

  bool operator==(const SymbolTableKey &key) const
  {
    return key.sym_name == this->sym_name && key.sym_type == this->sym_type;
  }
};

struct SymbolTableKeyHasher
{
  std::size_t operator()(const SymbolTableKey &k) const
  {
    using std::hash;
    using std::size_t;

    return hash<std::string>()(k.sym_name) ^ (hash<int>()(static_cast<int>(k.sym_type)) << 1);
  }
};

class Linker
{
private:
  uint32_t symbol_id = 0;

  std::vector<ObjectFile *> obj_files;
  std::unordered_map<std::string, uint32_t> section_places;
  std::unordered_map<uint32_t, type::byte> output_file_byte;
  std::string output_file_name = "";

  std::unordered_map<std::string, Interval> placed_sections;
  std::unordered_map<SymbolTableKey, SymbolJsonRecord, SymbolTableKeyHasher> global_sym_table;
  std::vector<RelocationJsonRecord> global_relocations;

  std::unordered_map<std::string, SectionJsonRecord> global_sections;

  // <sym_id, SymbolTableKey> pairs
  std::unordered_map<uint32_t, SymbolTableKey> sym_table_lookup_map;

  bool internal_err = false;

  Linker();
  Linker(const Linker &) = delete;
  Linker &operator=(const Linker &) = delete;
  ~Linker();

  bool add_placed_section(std::string section_name, Interval addr_int);
  std::string get_placed_section_by_addr(uint32_t addr);
  uint32_t generate_next_id();
  SymbolJsonRecord create_new_section(std::string section_name, uint32_t addr);
  void add_sym_to_table(SymbolJsonRecord sym);
  bool is_sym_in_table(std::string sym_name, type::PARAMETER_TYPE sym_type);
  bool is_sym_in_table(SymbolJsonRecord sym);
  bool is_sym_in_table(uint32_t sym_id);
  SymbolJsonRecord *get_sym_from_table(std::string sym_name, type::PARAMETER_TYPE sym_type);
  SymbolJsonRecord *get_sym_from_table(SymbolJsonRecord sym);
  SymbolJsonRecord *get_sym_from_table(uint32_t sym_id);
  std::vector<type::byte> resolve_relocation(RelocationJsonRecord relocation);
  bool place_sections(uint32_t max_mem_addr = 0);

public:
  static Linker &get_instance();

  bool create_hex();
  bool create_relocatable();
  void internal_error(std::string err_msg);

  void set_output_file_name(std::string file_name);
  bool load_input_obj_files(std::vector<std::string> input_file_names);

  void set_section_places(std::unordered_map<std::string, uint32_t> section_places);
  std::unordered_map<std::string, uint32_t> get_section_places();
};

#endif