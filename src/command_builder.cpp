#include "../inc/command_builder.hpp"
#include "../inc/all_instructions.hpp"
#include "../inc/all_directives.hpp"

CommandBuilder& CommandBuilder::get_instance()
{
  static CommandBuilder builder_instance;
  return builder_instance;
}

Instruction* CommandBuilder::build_instruction(type::INSTRUCTION_TYPE ins_alias)
{ // TODO: finish implementation of build_instruction method
  Instruction *ins = nullptr;

  switch(ins_alias)
  {
    case type::HALT: 
      {
        ins = new instruction::HALT();
        ins->set_params(this->get_params());
      }
      break;
    case type::INT:
      {
        ins = new instruction::INT();
      }
      break;
    case type::IRET:
      {
        
      }
      break;
    case type::CALL:
      {
        
      }
      break;
    case type::RET:
      {
        
      }
      break;
    case type::JMP:
      {
        
      }
      break;
    case type::BEQ:
      {
        
      }
      break;
    case type::BNE:
      {
        
      }
      break;
    case type::BGT:
      {
        
      }
      break;
    case type::PUSH:
      {
        
      }
      break;
    
    case type::POP:
      {
        
      }
      break;
    case type::XCHG:
      {
        
      }
      break;
    case type::ADD:
      {
        
      }
      break;
    case type::SUB:
      {
        
      }
      break;
    case type::MUL:
      {
        
      }
      break;
    case type::DIV:
      {
        
      }
      break;
    case type::NOT:
      {
        
      }
      break;
    case type::AND:
      {
        
      }
      break;
    case type::OR:
      {
        
      }
      break;
    case type::XOR:
      {
        
      }
      break;
    case type::SHL:
      {
        
      }
      break;
    case type::SHR:
      {
        
      }
      break;
    case type::LD:
      {
        
      }
      break;
    case type::ST:
      {
        
      }
      break;
    case type::CSRRD:
      {
        
      }
      break;
    case type::CSRWR:
      {
        
      }
      break;
  }

  // if instruction is successfully created, clear builder's params list so that
  // builder can be again used without explicit call of clear_params method
  if(ins) this->clear_params();
  return ins;
}

Directive* CommandBuilder::build_directive(type::DIRECTIVE_TYPE dir_alias)
{ // TODO: implement build_directive method
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
        
      }
      break;
    case type::WORD:
      {
        
      }
      break;
    case type::SKIP:
      {
        
      }
      break;
    case type::ASCII:
      {
        
      }
      break;
    case type::EQU:
      {
        
      }
      break;
    case type::END:
      {
        
      }
      break;
  }

  // if directive is successfully created, clear builder's params list so that
  // builder can be again used without explicit call of clear_params method
  if(dir) this->clear_params();
  return dir;
}

void CommandBuilder::set_gp_reg_0(type::GP_REG gp_reg) { this->gp_reg_0 = gp_reg; }
void CommandBuilder::set_gp_reg_1(type::GP_REG gp_reg) { this->gp_reg_1 = gp_reg; }
type::GP_REG CommandBuilder::set_gp_reg_0() const { return this->gp_reg_0; }
type::GP_REG CommandBuilder::set_gp_reg_1() const { return this->gp_reg_1; }

void CommandBuilder::set_cs_reg_0(type::CS_REG cs_reg) { this->cs_reg_0 = cs_reg; }
void CommandBuilder::set_cs_reg_1(type::CS_REG cs_reg) { this->cs_reg_1 = cs_reg; }
type::CS_REG CommandBuilder::set_cs_reg_0() const { return this->cs_reg_0; } 
type::CS_REG CommandBuilder::set_cs_reg_1() const { return this->cs_reg_1; }

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