#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include "command.hpp"
#include "parameter.hpp"
#include<list>

class Directive : public Command
{
private:
  std::list<Parameter> params;
public:
  Directive() : Command(type::INSTRUCTION) { }

  void set_params(std::list<Parameter> params);
  void enque_param(Parameter param);
  Parameter deque_param();
  std::list<Parameter> get_params() const;
};

#endif