#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "command.hpp"
#include "types.hpp"

class Instruction : public Command
{
private:
  type::MEMORY_ADDRESSING_MODES mem_addr_mode;
  type::GP_REG gp_reg_0, gp_reg_1;
  type::CS_REG cs_reg_0, cs_reg_1;

public:
  Instruction() : Command(type::INSTRUCTION) { }

  void set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES);
  type::MEMORY_ADDRESSING_MODES get_mem_addr_mode() const;

  void set_gp_reg_0(type::GP_REG gp_reg);
  void set_gp_reg_1(type::GP_REG gp_reg);
  type::GP_REG set_gp_reg_0() const; // XXX: maybe remove getters
  type::GP_REG set_gp_reg_1() const;

  void set_cs_reg_0(type::CS_REG cs_reg);
  void set_cs_reg_1(type::CS_REG cs_reg);
  type::CS_REG set_cs_reg_0() const; // XXX: maybe remove getters
  type::CS_REG set_cs_reg_1() const;

};

#endif