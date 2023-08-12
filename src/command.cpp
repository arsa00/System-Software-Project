#include "../inc/command.hpp"
#include <iostream>

void Command::destroy()
{
  Parameter *param;
  while((param = this->deque_param()))
  {
    std::cout << "Deleting parameter..." << std::endl;
    delete param;
  }
}

void Command::clone(const Command &cmd)
{
  std::list<Parameter *> cmd_params = cmd.get_params();

  for(Parameter* const& param : cmd_params) 
  {
    Parameter *new_param = new Parameter(*param);
    this->enque_param(new_param);
  }
}

void Command::move(Command &cmd)
{
  Parameter *cmd_param;
  while((cmd_param = cmd.deque_param()))
  {
    this->enque_param(new Parameter(*cmd_param));
  }
}

Command::Command(type::COMMAND_TYPE type) : type(type) 
{
  std::cout << "Def constructor..." << std::endl;
}

Command::Command(const Command &cmd)
{
  std::cout << "Copy constructor..." << std::endl;
  this->clone(cmd);
}

Command::Command(Command &&cmd)
{  
  std::cout << "Move constructor..." << std::endl;
  this->move(cmd);
}

Command& Command::operator=(const Command &cmd)
{
  std::cout << "Copy assigment operator..." << std::endl;
  if(this == &cmd) return *this;

  this->destroy();
  this->clone(cmd);
}

Command& Command::operator=(Command &&cmd)
{
  std::cout << "Move assigment operator..." << std::endl;
  if(this == &cmd) return *this;

  this->destroy();
  this->clone(cmd);
}

Command::~Command()
{
  this->destroy();
}

void Command::set_params(std::list<Parameter*> params)
{
  this->params = params;
}

void Command::enque_param(Parameter *param) 
{
  this->params.push_back(param);
}

Parameter* Command::deque_param()
{
  if(this->params.empty()) return nullptr;

  Parameter *param = this->params.front();
  this->params.pop_front();
  return param;
}

std::list<Parameter*> Command::get_params() const
{
  return this->params;
}