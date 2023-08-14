#include "../inc/command_builder.hpp"
#include "../inc/instructions/halt.hpp"

CommandBuilder& CommandBuilder::get_instance()
{
  static CommandBuilder builder_instance;
  return builder_instance;
}

Instruction* CommandBuilder::build_instruction(type::INSTRUCTION_TYPE ins_alias) const
{ // TODO: finish implementation of build_instruction method
  switch(ins_alias)
  {
    case type::HALT: 
      {
        Instruction *ins = new instruction::HALT();
        ins->set_params(this->get_params());
        return ins;
      }
      break;
  }
}

Directive* CommandBuilder::build_directive(type::DIRECTIVE_TYPE dir_alias) const
{ // TODO: implement build_directive method

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