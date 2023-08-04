#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "command.hpp"
#include "parameter.hpp"
#include<list>

class Instruction : public Command
{
private:
  std::list<Parameter> params;
  type::MEMORY_ADDRESSING_MODES mem_addr_mode;
public:
  Instruction() : Command(type::INSTRUCTION) { }

  void set_params(std::list<Parameter> params);
  void enque_param(Parameter param);
  Parameter deque_param();
  std::list<Parameter> get_params() const;

  void set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES);
  type::MEMORY_ADDRESSING_MODES get_mem_addr_mode() const;
};

#endif