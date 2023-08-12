#include "../inc/instruction.hpp"

void Instruction::set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES mode)
{
  this->mem_addr_mode = mode;
}

type::MEMORY_ADDRESSING_MODES Instruction::get_mem_addr_mode() const
{
  return this->mem_addr_mode;
}