#include "../inc/converters.hpp"

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