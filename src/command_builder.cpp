#include "../inc/command_builder.hpp"
#include "../inc/all_instructions.hpp"
#include "../inc/all_directives.hpp"
#include "../inc/assembler.hpp"
#include "../auxiliary/inc/converters.hpp"
#include <iostream>
// TODO: maybe add validation in building process

CommandBuilder &CommandBuilder::get_instance()
{
  static CommandBuilder builder_instance;
  return builder_instance;
}

void CommandBuilder::clear_builder()
{
  this->gp_reg_0 = type::GP_REG::NO_REG;
  this->gp_reg_1 = type::GP_REG::NO_REG;
  this->cs_reg_0 = type::CS_REG::NO_REG;
  this->cs_reg_1 = type::CS_REG::NO_REG;
  this->mem_addr_mode = type::MEMORY_ADDRESSING_MODES::NO_MODE;
  this->clear_params();
}

Instruction *CommandBuilder::build_instruction(type::INSTRUCTION_TYPE ins_alias)
{ // TODO: test build_instruction
  Instruction *ins = nullptr;

  switch (ins_alias)
  {
  case type::INSTRUCTION_TYPE::HALT:
  {
    ins = new instruction::HALT();
  }
  break;
  case type::INSTRUCTION_TYPE::INT:
  {
    ins = new instruction::INT();
  }
  break;
  case type::INSTRUCTION_TYPE::IRET:
  {
    ins = new instruction::IRET();
  }
  break;
  case type::INSTRUCTION_TYPE::CALL:
  {
    ins = new instruction::CALL();
    ins->set_params(this->get_params());
  }
  break;
  case type::INSTRUCTION_TYPE::RET:
  {
    ins = new instruction::RET();
  }
  break;
  case type::INSTRUCTION_TYPE::JMP:
  {
    ins = new instruction::JMP();
    ins->set_params(this->get_params());
  }
  break;
  case type::INSTRUCTION_TYPE::BEQ:
  {
    ins = new instruction::BEQ();
    ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
    ins->set_params(this->get_params());
  }
  break;
  case type::INSTRUCTION_TYPE::BNE:
  {
    ins = new instruction::BNE();
    ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
    ins->set_params(this->get_params());
  }
  break;
  case type::INSTRUCTION_TYPE::BGT:
  {
    ins = new instruction::BGT();
    ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
    ins->set_params(this->get_params());
  }
  break;
  case type::INSTRUCTION_TYPE::PUSH:
  {
    ins = new instruction::PUSH();
    ins->set_gp_reg_0(this->gp_reg_0);
  }
  break;

  case type::INSTRUCTION_TYPE::POP:
  {
    ins = new instruction::POP();
    ins->set_gp_reg_0(this->gp_reg_0);
  }
  break;
  case type::INSTRUCTION_TYPE::XCHG:
  {
    ins = new instruction::XCHG();
    ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
  }
  break;
  case type::INSTRUCTION_TYPE::ADD:
  {
    ins = new instruction::ADD();
    ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
  }
  break;
  case type::INSTRUCTION_TYPE::SUB:
  {
    ins = new instruction::SUB();
    ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
  }
  break;
  case type::INSTRUCTION_TYPE::MUL:
  {
    ins = new instruction::MUL();
    ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
  }
  break;
  case type::INSTRUCTION_TYPE::DIV:
  {
    ins = new instruction::DIV();
    ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
  }
  break;
  case type::INSTRUCTION_TYPE::NOT:
  {
    ins = new instruction::NOT();
    ins->set_gp_reg_0(this->gp_reg_0);
  }
  break;
  case type::INSTRUCTION_TYPE::AND:
  {
    ins = new instruction::AND();
    ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
  }
  break;
  case type::INSTRUCTION_TYPE::OR:
  {
    ins = new instruction::OR();
    ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
  }
  break;
  case type::INSTRUCTION_TYPE::XOR:
  {
    ins = new instruction::XOR();
    ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
  }
  break;
  case type::INSTRUCTION_TYPE::SHL:
  {
    ins = new instruction::SHL();
    ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
  }
  break;
  case type::INSTRUCTION_TYPE::SHR:
  {
    ins = new instruction::SHR();
    ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
  }
  break;
  case type::INSTRUCTION_TYPE::LD:
  {
    ins = new instruction::LD();
    ins->set_mem_addr_mode(this->mem_addr_mode);
    ins->set_gp_reg_0(this->gp_reg_0);
    ins->set_gp_reg_1(this->gp_reg_1); // only for REG_DIR, REG_IND & REG_IND_WITH_DISP, but it will not hurt to set it every time
    ins->set_params(this->get_params());
  }
  break;
  case type::INSTRUCTION_TYPE::ST:
  {
    ins = new instruction::ST();
    ins->set_mem_addr_mode(this->mem_addr_mode);
    ins->set_gp_reg_0(this->gp_reg_0);
    ins->set_params(this->get_params());

    // unallowed mem_addr_mode is set
    if (this->mem_addr_mode == type::MEMORY_ADDRESSING_MODES::IMMED)
      Assembler::get_instance().parse_error("Error occured while building " + converter::instruction_type_to_string(ins_alias) + " instruction. \
Unallowed combination of memory addressing mode and instruction parameters.");
  }
  break;
  case type::INSTRUCTION_TYPE::CSRRD:
  {
    ins = new instruction::CSRRD();
    ins->set_gp_reg_0(this->gp_reg_0);
    ins->set_cs_reg_0(this->cs_reg_0);
  }
  break;
  case type::INSTRUCTION_TYPE::CSRWR:
  {
    ins = new instruction::CSRWR();
    ins->set_gp_reg_0(this->gp_reg_0);
    ins->set_cs_reg_0(this->cs_reg_0);
  }
  break;
  }

  // if instruction is successfully created, clear builder's params list so that
  // builder can be again used without explicit call of clear_params method
  this->clear_builder();

  if (!ins)
    Assembler::get_instance().parse_error("Error occured while building " + converter::instruction_type_to_string(ins_alias) + " instruction");

  return ins;
}

Directive *CommandBuilder::build_directive(type::DIRECTIVE_TYPE dir_alias)
{ // TODO: test build_directive
  Directive *dir = nullptr;
  bool directive_executed = false; // for directives that are executed in the first round

  switch (dir_alias)
  {
  case type::DIRECTIVE_TYPE::GLOBAL:
  {
    dir = new directive::GLOBAL();
    dir->set_params(this->get_params());
  }
  break;
  case type::DIRECTIVE_TYPE::EXTERN:
  { // this directive is executed in first round of assembling
    // XXX: maybe check if Parameter is instance of Symbol
    Symbol *sym = (Symbol *)this->deque_param();
    sym->set_global_flag(true);
    sym->set_defined_flag(true);
    directive_executed = true;
  }
  break;
  case type::DIRECTIVE_TYPE::SECTION:
  { // this directive is executed in first round of assembling
    // XXX: maybe check if Parameter is instance of Symbol
    Symbol *sym = (Symbol *)this->deque_param();
    std::cout << "[CMD_BUILDER->.section]: " << sym->get_name() << std::endl;
    Assembler::get_instance().add_section(sym->get_name());
    directive_executed = true;
  }
  break;
  case type::DIRECTIVE_TYPE::WORD:
  {
    dir = new directive::WORD();
    dir->set_params(this->get_params());
  }
  break;
  case type::DIRECTIVE_TYPE::SKIP:
  {
    dir = new directive::SKIP();
    dir->set_params(this->get_params());
  }
  break;
  case type::DIRECTIVE_TYPE::ASCII:
  {
    dir = new directive::ASCII();
    dir->set_params(this->get_params());
  }
  break;
  case type::DIRECTIVE_TYPE::END:
  {
    dir = new directive::END();
  }
  break;
    // TODO: maybe add EQU?
  }

  // clear builder's params list so that builder can be again used,
  // without explicit call of clear_params method
  this->clear_builder();

  if (!dir && !directive_executed)
    Assembler::get_instance().parse_error("Error occured while building " + converter::directive_type_to_string(dir_alias) + " directive");

  std::cout << "[CMD_BUILDER]: finished" << std::endl;
  return dir;
}

void CommandBuilder::set_next_gp_reg(type::GP_REG gp_reg)
{
  if (this->gp_reg_0 != type::GP_REG::NO_REG)
    this->gp_reg_0 = gp_reg;

  if (this->gp_reg_1 != type::GP_REG::NO_REG)
    this->gp_reg_1 = gp_reg;
}

void CommandBuilder::set_gp_regs(type::GP_REG gp_reg_0, type::GP_REG gp_reg_1)
{
  this->gp_reg_0 = gp_reg_0;
  this->gp_reg_1 = gp_reg_1;
}

void CommandBuilder::set_gp_reg_0(type::GP_REG gp_reg)
{
  this->gp_reg_0 = gp_reg;
}

void CommandBuilder::set_gp_reg_1(type::GP_REG gp_reg)
{
  this->gp_reg_1 = gp_reg;
}

type::GP_REG CommandBuilder::get_gp_reg_0() const
{
  return this->gp_reg_0;
}

type::GP_REG CommandBuilder::get_gp_reg_1() const
{
  return this->gp_reg_1;
}

void CommandBuilder::set_next_cs_reg(type::CS_REG cs_reg)
{
  if (this->cs_reg_0 != type::CS_REG::NO_REG)
    this->cs_reg_0 = cs_reg;

  if (this->cs_reg_1 != type::CS_REG::NO_REG)
    this->cs_reg_1 = cs_reg;
}

void CommandBuilder::set_cs_regs(type::CS_REG cs_reg_0, type::CS_REG cs_reg_1)
{
  this->cs_reg_0 = cs_reg_0;
  this->cs_reg_1 = cs_reg_1;
}

void CommandBuilder::set_cs_reg_0(type::CS_REG cs_reg)
{
  this->cs_reg_0 = cs_reg;
}

void CommandBuilder::set_cs_reg_1(type::CS_REG cs_reg)
{
  this->cs_reg_1 = cs_reg;
}

type::CS_REG CommandBuilder::get_cs_reg_0() const
{
  return this->cs_reg_0;
}

type::CS_REG CommandBuilder::get_cs_reg_1() const
{
  return this->cs_reg_1;
}

void CommandBuilder::set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES mode)
{
  this->mem_addr_mode = mode;
}

void CommandBuilder::set_params(const std::list<Parameter *> &params)
{
  this->params = params;
}

void CommandBuilder::clear_params()
{
  this->params.clear();
}

void CommandBuilder::delete_params()
{
  Parameter *param;
  while ((param = this->deque_param()))
    delete param;
}

std::list<Parameter *> CommandBuilder::get_params() const
{
  return this->params;
}

void CommandBuilder::enque_param(Parameter *param)
{
  this->params.push_back(param);
}

Parameter *CommandBuilder::deque_param()
{
  if (this->params.empty())
    return nullptr;

  Parameter *param = this->params.front();
  this->params.pop_front();
  return param;
}