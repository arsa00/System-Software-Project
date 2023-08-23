#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "types.hpp"
#include "parameter.hpp"
#include "section.hpp"
#include <list>

class Command
{
private:
  void destroy();
  void clone(const Command &);
  void move(Command &);

protected:
  /* XXX:
    params list have original symbols (pointers) from Assembler's symbol_table,
    so Command class is NOT responsible for cleaning memory of symbols in params list.
    However, Command class is fully responsible cleaning memory of all literals in it's params list.
  */
  std::list<Parameter *> params;
  type::COMMAND_TYPE type;
  unsigned int size = 0;
  bool is_generating_data = true;

public:
  Command(type::COMMAND_TYPE type, unsigned int size = 0);

  Command(const Command &);
  Command(Command &&);
  Command &operator=(const Command &);
  Command &operator=(Command &&);
  ~Command();

  void set_params(const std::list<Parameter *> &params);
  std::list<Parameter *> get_params() const;
  void delete_params();
  void clear_params();
  void enque_param(Parameter *param);
  Parameter *deque_param();

  void set_size(unsigned int);
  unsigned int get_size() const;

  void set_generate_data_flag(bool flag_value);
  bool get_generate_data_flag();

  virtual void execute(Section *dest_section) const = 0;
};

#endif