#ifndef CONVERTERS_HPP
#define CONVERTERS_HPP

#include "../../inc/types.hpp"
#include <string>
#include <array>
#include <vector>

namespace converter
{

  std::string directive_type_to_string(type::DIRECTIVE_TYPE dir_alias);
  std::string instruction_type_to_string(type::INSTRUCTION_TYPE ins_alias);
  std::string cpu_instruction_type_to_string(type::CPU_INSTRUCTIONS ins_alias);
  std::string relocation_type_to_string(type::RELOCATIONS rel_alias);
  std::string uint32_to_hex_string(uint32_t value);

  std::array<type::byte, 2> disp_to_byte_arr(int16_t displacement); // returns byte array with 2 elements
  int16_t get_disp_from_instruction(type::instruction_size instruction_record);
  int16_t get_negative_val_disp(uint16_t disp_val);

  type::byte get_upper_half_byte(type::byte single_byte); // TODO: test
  type::byte get_lower_half_byte(type::byte single_byte); // TODO: test

  void write_to_upper_byte_half(type::byte new_value, type::byte *original_byte); // TODO: test
  void write_to_lower_byte_half(type::byte new_value, type::byte *original_byte); // TODO: test
  type::byte create_byte_of_two_halves(type::byte upper_half, type::byte lower_half);

  type::instruction_size create_instruction_of_bytes(type::byte b3, type::byte b2, type::byte b1, type::byte b0);
  std::array<type::byte, 4> get_instruction_bytes(type::instruction_size instruction);

  std::string get_value_from_json(const std::string &json_file, const std::string &key, uint32_t *start_pos = nullptr, bool is_array = false);
  std::vector<std::string> decode_json_array(const std::string &json_arr);
}

#endif