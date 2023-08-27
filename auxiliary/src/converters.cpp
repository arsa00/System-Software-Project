#include "../inc/converters.hpp"
#include <iostream>

std::string converter::directive_type_to_string(type::DIRECTIVE_TYPE dir_alias)
{
  switch (dir_alias)
  {
  case type::DIRECTIVE_TYPE::GLOBAL:
    return ".global";
  case type::DIRECTIVE_TYPE::EXTERN:
    return ".extern";
  case type::DIRECTIVE_TYPE::SECTION:
    return ".section";
  case type::DIRECTIVE_TYPE::WORD:
    return ".word";
  case type::DIRECTIVE_TYPE::SKIP:
    return ".skip";
  case type::DIRECTIVE_TYPE::ASCII:
    return ".ascii";
  case type::DIRECTIVE_TYPE::END:
    return ".end";
  case type::DIRECTIVE_TYPE::EQU:
    return ".equ";
  default:
    return "";
  }
}

std::string converter::instruction_type_to_string(type::INSTRUCTION_TYPE ins_alias)
{
  switch (ins_alias)
  {
  case type::INSTRUCTION_TYPE::HALT:
    return "HALT";
  case type::INSTRUCTION_TYPE::INT:
    return "INT";
  case type::INSTRUCTION_TYPE::IRET:
    return "IRET";
  case type::INSTRUCTION_TYPE::CALL:
    return "CALL";
  case type::INSTRUCTION_TYPE::RET:
    return "RET";
  case type::INSTRUCTION_TYPE::JMP:
    return "JMP";
  case type::INSTRUCTION_TYPE::BEQ:
    return "BEQ";
  case type::INSTRUCTION_TYPE::BNE:
    return "BNE";
  case type::INSTRUCTION_TYPE::BGT:
    return "BGT";
  case type::INSTRUCTION_TYPE::PUSH:
    return "PUSH";
  case type::INSTRUCTION_TYPE::POP:
    return "POP";
  case type::INSTRUCTION_TYPE::XCHG:
    return "XCHG";
  case type::INSTRUCTION_TYPE::ADD:
    return "ADD";
  case type::INSTRUCTION_TYPE::SUB:
    return "SUB";
  case type::INSTRUCTION_TYPE::MUL:
    return "MUL";
  case type::INSTRUCTION_TYPE::DIV:
    return "DIV";
  case type::INSTRUCTION_TYPE::NOT:
    return "NOT";
  case type::INSTRUCTION_TYPE::AND:
    return "AND";
  case type::INSTRUCTION_TYPE::OR:
    return "OR";
  case type::INSTRUCTION_TYPE::XOR:
    return "XOR";
  case type::INSTRUCTION_TYPE::SHL:
    return "SHL";
  case type::INSTRUCTION_TYPE::SHR:
    return "SHR";
  case type::INSTRUCTION_TYPE::LD:
    return "LD";
  case type::INSTRUCTION_TYPE::ST:
    return "ST";
  case type::INSTRUCTION_TYPE::CSRRD:
    return "CSRRD";
  case type::INSTRUCTION_TYPE::CSRWR:
    return "CSRWR";
  default:
    return "";
  }
}

// displacement is 12bit value, so higher 4 bits of arr[0] are always zero and must be overwritten
// to use value needed in instruction
std::array<type::byte, 2> converter::disp_to_byte_arr(int16_t displacement)
{
  std::array<type::byte, 2> byte_arr;
  byte_arr[0] = displacement & 0x000F; // lowest 4 bits and 4 zeros padding at upper half
  byte_arr[1] = (displacement >> 4) & 0x00FF;

  return byte_arr;
}

int16_t converter::get_disp_from_instruction(type::instruction_size instruction_record)
{
  type::byte byte4 = (instruction_record >> 24) & 0x000000FF; // get 4th byte of ins
  type::byte byte3 = (instruction_record >> 16) & 0x000000FF; // get 3rd byte of ins

  uint16_t disp_val = ((byte4 << 4) & 0xFFF0) | (byte3 & 0x0F); // combine 4th byte and lower 4 bits of 3rd byte
  int16_t disp_singed_val;

  // check if displacement value is negative
  if (disp_val & 0x0800)
    disp_singed_val = converter::get_negative_val_disp(disp_val);
  else
    disp_singed_val = disp_val;

  return disp_singed_val;
}

int16_t converter::get_negative_val_disp(uint16_t disp_val)
{
  int16_t singed_val = 0xF000 | disp_val;
  return singed_val;
}

type::byte converter::get_upper_half_byte(type::byte single_byte)
{
  return (single_byte & 0xF0) >> 4;
}

type::byte converter::get_lower_half_byte(type::byte single_byte)
{
  return single_byte & 0x0F;
}

void converter::write_to_upper_byte_half(type::byte new_value, type::byte *original_byte)
{
  (*original_byte) &= 0x0F; // clear upper 4 bits
  new_value <<= 4;          // shift lower 4 bits to upper position
  new_value &= 0xF0;        // clear lower 4 bits (just in case)

  (*original_byte) |= new_value; // write new 4 bits to higher 4 bits of original value
}

void converter::write_to_lower_byte_half(type::byte new_value, type::byte *original_byte)
{
  (*original_byte) &= 0xF0; // clear lower 4 bits
  new_value &= 0x0F;        // clear upper 4 bits

  (*original_byte) |= new_value; // write new 4 bits to lower 4 bits of original value
}

type::byte converter::create_byte_of_two_halves(type::byte upper_half, type::byte lower_half)
{
  type::byte new_byte = (upper_half << 4) & 0xF0; // write 4 bits to higher half
  new_byte |= (lower_half & 0x0F);                // write 4 bits to lower half

  return new_byte;
}

type::instruction_size converter::create_instruction_of_bytes(type::byte b3, type::byte b2, type::byte b1, type::byte b0)
{
  type::instruction_size ins_record = 0x00000000;

  ins_record |= ((b3 << 24) & 0xFF000000);
  ins_record |= ((b2 << 16) & 0x00FF0000);
  ins_record |= ((b1 << 8) & 0x0000FF00);
  ins_record |= (b0 & 0x000000FF);

  return ins_record;
}
