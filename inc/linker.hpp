#ifndef LINKER_HPP
#define LINKER_HPP

#include "../auxiliary/inc/object_file.hpp"
#include <string>
#include <unordered_map>
#include <list>
#include <vector>

class Linker
{
private:
  std::vector<ObjectFile *> obj_files;
  std::unordered_map<std::string, uint32_t> section_places;
  std::unordered_map<uint32_t, type::byte> output_file_byte;
  std::string output_file_name = "";

  bool internal_err = false;

  Linker();
  Linker(const Linker &) = delete;
  Linker &operator=(const Linker &) = delete;
  ~Linker();

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