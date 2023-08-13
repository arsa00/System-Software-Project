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
  
  void set_params(const std::list<Parameter*> &params);
  std::list<Parameter*> get_params() const;
  void delete_params();
  void clear_params();
  void enque_param(Parameter *param);
  Parameter* deque_param();
  

  virtual void execute() const = 0;
};

#endif