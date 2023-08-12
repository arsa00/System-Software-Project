#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "command.hpp"

class Instruction : public Command
{
private:
  type::MEMORY_ADDRESSING_MODES mem_addr_mode;

public:
  Instruction() : Command(type::INSTRUCTION) { }

  void set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES);
  type::MEMORY_ADDRESSING_MODES get_mem_addr_mode() const;
};

#endif