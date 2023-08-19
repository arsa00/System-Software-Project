#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include "command.hpp"

class Directive : public Command
{
private:
public:
  Directive() : Command(type::COMMAND_TYPE::DIRECTIVE) {}
};

#endif