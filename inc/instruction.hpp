#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "command.hpp"
#include "types.hpp"

class Instruction : public Command
{
private:
  type::MEMORY_ADDRESSING_MODES mem_addr_mode = type::MEMORY_ADDRESSING_MODES::NO_MODE;
  type::GP_REG gp_reg_0, gp_reg_1;
  type::CS_REG cs_reg_0, cs_reg_1;

public:
  Instruction() : Command(type::COMMAND_TYPE::INSTRUCTION, type::DEFAULT_INSTRUCTION_SIZE) {}

  void set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES);
  type::MEMORY_ADDRESSING_MODES get_mem_addr_mode() const;

  void set_gp_reg_0(type::GP_REG gp_reg);
  void set_gp_reg_1(type::GP_REG gp_reg);
  void set_gp_regs(type::GP_REG gp_reg_0, type::GP_REG gp_reg_1);
  type::GP_REG get_gp_reg_0() const; // XXX: maybe remove getters
  type::GP_REG get_gp_reg_1() const;

  void set_cs_reg_0(type::CS_REG cs_reg);
  void set_cs_reg_1(type::CS_REG cs_reg);
  void set_cs_regs(type::CS_REG cs_reg_0, type::CS_REG cs_reg_1);
  type::CS_REG get_cs_reg_0() const; // XXX: maybe remove getters
  type::CS_REG get_cs_reg_1() const;
};

#endif