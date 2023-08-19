#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include "instruction.hpp"
#include "directive.hpp"
#include "types.hpp"
#include "symbol.hpp"

#include <string>
#include <unordered_map>
#include <list>

class Assembler
{
private:
  std::unordered_map<std::string, Symbol *> symbol_table;
  std::list<Command *> all_commands;
  unsigned int location_counter = 0;

  // TODO: add object file field and (maybe?) ObjectFile class

  Assembler() {}
  Assembler(const Assembler &) = delete;
  Assembler &operator=(const Assembler &) = delete;

public:
  static Assembler &get_instance();

  bool run();
  void add_symbol(Symbol *);
  void add_command(Command *);
};

#endif