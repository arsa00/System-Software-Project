#include "../inc/command.hpp"
#include <iostream>

void Command::destroy()
{
  Parameter *param;
  while ((param = this->deque_param()))
  {
    // std::cout << "Deleting parameter..." << std::endl;
    if (param->get_type() == type::PARAMETER_TYPE::LITERAL)
      delete param;
  }
}

void Command::clone(const Command &cmd)
{
  std::list<Parameter *> cmd_params = cmd.get_params();

  for (Parameter *const &param : cmd_params)
  {
    Parameter *new_param = param->clone();
    this->enque_param(new_param);
  }
}

void Command::move(Command &cmd)
{
  Parameter *cmd_param;
  while ((cmd_param = cmd.deque_param()))
  {
    this->enque_param(cmd_param->clone());
  }
}

Command::Command(type::COMMAND_TYPE type, uint32_t size)
{
  // std::cout << "Def constructor..." << std::endl;
  this->type = type;
  this->size = size;
}

Command::Command(const Command &cmd)
{
  // std::cout << "Copy constructor..." << std::endl;
  this->clone(cmd);
}

Command::Command(Command &&cmd)
{
  // std::cout << "Move constructor..." << std::endl;
  this->move(cmd);
}

Command &Command::operator=(const Command &cmd)
{
  // std::cout << "Copy assigment operator..." << std::endl;
  if (this == &cmd)
    return *this;

  this->destroy();
  this->clone(cmd);
}

Command &Command::operator=(Command &&cmd)
{
  // std::cout << "Move assigment operator..." << std::endl;
  if (this == &cmd)
    return *this;

  this->destroy();
  this->clone(cmd);
}

Command::~Command()
{
  this->destroy();
}

void Command::set_params(const std::list<Parameter *> &params)
{
  this->params = params;
}

void Command::clear_params()
{
  this->params.clear();
}

void Command::delete_params()
{
  this->destroy();
}

std::list<Parameter *> Command::get_params() const
{
  return this->params;
}

void Command::enque_param(Parameter *param)
{
  this->params.push_back(param);
}

Parameter *Command::deque_param()
{
  if (this->params.empty())
    return nullptr;

  Parameter *param = this->params.front();
  this->params.pop_front();
  return param;
}

void Command::set_size(uint32_t size)
{
  this->size = size;
}

// This method must not be called before all arguments (params, registers, etc.) are set.
// In other case, it's value is not reliable.
uint32_t Command::get_size() const
{
  return this->size;
}

void Command::set_generate_data_flag(bool flag_value)
{
  this->is_generating_data = flag_value;
}

bool Command::get_generate_data_flag()
{
  return this->is_generating_data;
}
