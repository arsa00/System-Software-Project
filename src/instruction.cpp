#include "../inc/instruction.hpp"

void Instruction::set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES mode)
{
  this->mem_addr_mode = mode;
}

type::MEMORY_ADDRESSING_MODES Instruction::get_mem_addr_mode() const
{
  return this->mem_addr_mode;
}

void Instruction::set_gp_reg_0(type::GP_REG gp_reg) { this->gp_reg_0 = gp_reg; }
void Instruction::set_gp_reg_1(type::GP_REG gp_reg) { this->gp_reg_1 = gp_reg; }
type::GP_REG Instruction::set_gp_reg_0() const { return this->gp_reg_0; }
type::GP_REG Instruction::set_gp_reg_1() const { return this->gp_reg_1; }

void Instruction::set_cs_reg_0(type::CS_REG cs_reg) { this->cs_reg_0 = cs_reg; }
void Instruction::set_cs_reg_1(type::CS_REG cs_reg) { this->cs_reg_1 = cs_reg; }
type::CS_REG Instruction::set_cs_reg_0() const { return this->cs_reg_0; } 
type::CS_REG Instruction::set_cs_reg_1() const { return this->cs_reg_1; }