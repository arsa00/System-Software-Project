#include "../inc/instruction.hpp"

void Instruction::set_params(std::list<Parameter> params)
{
  this->params = params;
}

void Instruction::enque_param(Parameter param) 
{
  this->params.push_back(param);
}

Parameter Instruction::deque_param()
{
  Parameter param = this->params.back();
  this->params.pop_back();
  return param;
}

std::list<Parameter> Instruction::get_params() const
{
  return this->params;
}

void Instruction::set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES mode)
{
  this->mem_addr_mode = mode;
}

type::MEMORY_ADDRESSING_MODES Instruction::get_mem_addr_mode() const
{
  return this->mem_addr_mode;
}