#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "types.hpp"

class Command
{
private:
  type::COMMAND_TYPE type;
public:
  Command(type::COMMAND_TYPE type) : type(type) { }
  virtual void execute() = 0;
};

#endif