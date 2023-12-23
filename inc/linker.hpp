#ifndef LINKER_HPP
#define LINKER_HPP

#include "../auxiliary/inc/object_file.hpp"
#include <string>
#include <unordered_map>
#include <list>
#include <vector>

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

class Linker
{
private:
  uint32_t symbol_id = 0;

  std::vector<ObjectFile *> obj_files;
  std::unordered_map<std::string, uint32_t> section_places;
  std::unordered_map<uint32_t, type::byte> output_file_byte;
  std::string output_file_name = "";

  std::unordered_map<std::string, Interval> placed_sections;
  std::unordered_map<std::string, SymbolJsonRecord> global_sym_table;
  std::vector<RelocationJsonRecord> global_relocations;

  bool internal_err = false;

  Linker();
  Linker(const Linker &) = delete;
  Linker &operator=(const Linker &) = delete;
  ~Linker();

  bool add_placed_section(std::string section_name, Interval addr_int);
  std::string get_placed_section_by_addr(uint32_t addr);
  uint32_t generate_next_id();
  SymbolJsonRecord create_new_section(std::string section_name, uint32_t addr);

public:
  static Linker &get_instance();

  std::string create_hex();
  std::string create_relocatable();
  void internal_error(std::string err_msg);

  void set_output_file_name(std::string file_name);
  bool load_input_obj_files(std::vector<std::string> input_file_names);

  void set_section_places(std::unordered_map<std::string, uint32_t> section_places);
  std::unordered_map<std::string, uint32_t> get_section_places();
};

#endif