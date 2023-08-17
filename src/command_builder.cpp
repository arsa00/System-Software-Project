#include "../inc/command_builder.hpp"
#include "../inc/all_instructions.hpp"
#include "../inc/all_directives.hpp"

CommandBuilder& CommandBuilder::get_instance()
{
  static CommandBuilder builder_instance;
  return builder_instance;
}

Instruction* CommandBuilder::build_instruction(type::INSTRUCTION_TYPE ins_alias)
{ // TODO: test build_instruction
  Instruction *ins = nullptr;

  switch(ins_alias)
  {
    case type::HALT: 
      {
        ins = new instruction::HALT();
      }
      break;
    case type::INT:
      {
        ins = new instruction::INT();
      }
      break;
    case type::IRET:
      {
        ins = new instruction::IRET();
      }
      break;
    case type::CALL:
      {
        ins = new instruction::CALL();
        ins->set_params(this->get_params());
      }
      break;
    case type::RET:
      {
        ins = new instruction::RET();
      }
      break;
    case type::JMP:
      {
        ins = new instruction::JMP();
        ins->set_params(this->get_params());
      }
      break;
    case type::BEQ:
      {
        ins = new instruction::BEQ();
        ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
        ins->set_params(this->get_params());
      }
      break;
    case type::BNE:
      {
        ins = new instruction::BNE();
        ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
        ins->set_params(this->get_params());
      }
      break;
    case type::BGT:
      {
        ins = new instruction::BGT();
        ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
        ins->set_params(this->get_params());
      }
      break;
    case type::PUSH:
      {
        ins = new instruction::PUSH();
        ins->set_gp_reg_0(this->gp_reg_0);
      }
      break;
    
    case type::POP:
      {
        ins = new instruction::POP();
        ins->set_gp_reg_0(this->gp_reg_0);
      }
      break;
    case type::XCHG:
      {
        ins = new instruction::XCHG();
        ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
      }
      break;
    case type::ADD:
      {
        ins = new instruction::ADD();
        ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
      }
      break;
    case type::SUB:
      {
        ins = new instruction::SUB();
        ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
      }
      break;
    case type::MUL:
      {
        ins = new instruction::MUL();
        ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
      }
      break;
    case type::DIV:
      {
        ins = new instruction::DIV();
        ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
      }
      break;
    case type::NOT:
      {
        ins = new instruction::NOT();
        ins->set_gp_reg_0(this->gp_reg_0);
      }
      break;
    case type::AND:
      {
        ins = new instruction::AND();
        ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
      }
      break;
    case type::OR:
      {
        ins = new instruction::OR();
        ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
      }
      break;
    case type::XOR:
      {
        ins = new instruction::XOR();
        ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
      }
      break;
    case type::SHL:
      {
        ins = new instruction::SHL();
        ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
      }
      break;
    case type::SHR:
      {
        ins = new instruction::SHR();
        ins->set_gp_regs(this->gp_reg_0, this->gp_reg_1);
      }
      break;
    case type::LD:
      {
        ins = new instruction::LD();
        ins->set_mem_addr_mode(this->mem_addr_mode);
        ins->set_gp_reg_0(this->gp_reg_0);
        ins->set_params(this->get_params());
      }
      break;
    case type::ST:
      {
        ins = new instruction::ST();
        ins->set_mem_addr_mode(this->mem_addr_mode);
        ins->set_gp_reg_0(this->gp_reg_0);
        ins->set_params(this->get_params());
      }
      break;
    case type::CSRRD:
      {
        ins = new instruction::CSRRD();
        ins->set_gp_reg_0(this->gp_reg_0);
        ins->set_cs_reg_0(this->cs_reg_0);
      }
      break;
    case type::CSRWR:
      {
        ins = new instruction::CSRWR();
        ins->set_gp_reg_0(this->gp_reg_0);
        ins->set_cs_reg_0(this->cs_reg_0);
      }
      break;
  }

  // if instruction is successfully created, clear builder's params list so that
  // builder can be again used without explicit call of clear_params method
  if(ins) this->clear_params();
  return ins;
}

Directive* CommandBuilder::build_directive(type::DIRECTIVE_TYPE dir_alias)
{ // TODO: test build_directive
  Directive *dir = nullptr;

  switch (dir_alias)
  {
    case type::GLOBAL:
      {
        dir = new directive::GLOBAL();
        dir->set_params(this->get_params());
      }
      break;
    case type::EXTERN:
      {
        dir = new directive::EXTERN();
        dir->set_params(this->get_params());
      }
      break;
    case type::SECTION:
      {
        dir = new directive::SECTION();
        dir->set_params(this->get_params());
      }
      break;
    case type::WORD:
      {
        dir = new directive::WORD();
        dir->set_params(this->get_params());
      }
      break;
    case type::SKIP:
      {
        dir = new directive::SKIP();
        dir->set_params(this->get_params());
      }
      break;
    case type::ASCII:
      {
        dir = new directive::ASCII();
        dir->set_params(this->get_params());
      }
      break;
    case type::END:
      {
        dir = new directive::END();
      }
      break;
    // TODO: maybe add EQU?
  }

  // if directive is successfully created, clear builder's params list so that
  // builder can be again used without explicit call of clear_params method
  if(dir) this->clear_params();
  return dir;
}

void CommandBuilder::set_gp_regs(type::GP_REG gp_reg_0, type::GP_REG gp_reg_1)
{ 
  this->gp_reg_0 = gp_reg_0; 
  this->gp_reg_1 = gp_reg_1; 
}
void CommandBuilder::set_gp_reg_0(type::GP_REG gp_reg) { this->gp_reg_0 = gp_reg; }
void CommandBuilder::set_gp_reg_1(type::GP_REG gp_reg) { this->gp_reg_1 = gp_reg; }
type::GP_REG CommandBuilder::get_gp_reg_0() const { return this->gp_reg_0; }
type::GP_REG CommandBuilder::get_gp_reg_1() const { return this->gp_reg_1; }

void CommandBuilder::set_cs_regs(type::CS_REG cs_reg_0, type::CS_REG cs_reg_1)
{ 
  this->cs_reg_0 = cs_reg_0; 
  this->cs_reg_1 = cs_reg_1; 
}
void CommandBuilder::set_cs_reg_0(type::CS_REG cs_reg) { this->cs_reg_0 = cs_reg; }
void CommandBuilder::set_cs_reg_1(type::CS_REG cs_reg) { this->cs_reg_1 = cs_reg; }
type::CS_REG CommandBuilder::get_cs_reg_0() const { return this->cs_reg_0; } 
type::CS_REG CommandBuilder::get_cs_reg_1() const { return this->cs_reg_1; }

void CommandBuilder::set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES mode)
{
  this->mem_addr_mode = mode;
}

void CommandBuilder::set_params(const std::list<Parameter*> &params)
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
  while((param = this->deque_param())) delete param;
}

std::list<Parameter*> CommandBuilder::get_params() const
{
  return this->params;
}

void CommandBuilder::enque_param(Parameter *param) 
{
  this->params.push_back(param);
}

Parameter* CommandBuilder::deque_param()
{
  if(this->params.empty()) return nullptr;

  Parameter *param = this->params.front();
  this->params.pop_front();
  return param;
}