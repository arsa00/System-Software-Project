#ifndef COMMAND_BUILDER_HPP
#define COMMAND_BUILDER_HPP

#include "instruction.hpp"
#include "directive.hpp"
#include "types.hpp"

class CommandBuilder
{
private:
  std::list<Parameter*> params;
  type::MEMORY_ADDRESSING_MODES mem_addr_mode;
  type::GP_REG gp_reg_0, gp_reg_1;
  type::CS_REG cs_reg_0, cs_reg_1;

  CommandBuilder() { }
  CommandBuilder(const CommandBuilder &) = delete;
  CommandBuilder& operator= (const CommandBuilder &) = delete;
public:
  static CommandBuilder& get_instance();
  Instruction* build_instruction(type::INSTRUCTION_TYPE);
  Directive* build_directive(type::DIRECTIVE_TYPE);

  void set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES);

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

  void set_params(const std::list<Parameter*> &params);
  std::list<Parameter*> get_params() const;
  void delete_params();
  void clear_params();
  void enque_param(Parameter *param);
  Parameter* deque_param();
};

#endif