#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "types.hpp"
#include "parameter.hpp"
#include<list>

class Command
{
private:
  void destroy();
  void clone(const Command &);
  void move(Command &);

protected:
  type::COMMAND_TYPE type;
  std::list<Parameter*> params;

public:
  Command(type::COMMAND_TYPE type);

  Command(const Command &);
  Command(Command &&);
  Command& operator=(const Command &);
  Command& operator=(Command &&);
  ~Command();
  
  void set_params(std::list<Parameter*> params);
  void enque_param(Parameter *param);
  Parameter* deque_param();
  std::list<Parameter*> get_params() const;

  virtual void execute() const = 0;
};

#endif