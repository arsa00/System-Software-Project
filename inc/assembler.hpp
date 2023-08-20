#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include "instruction.hpp"
#include "directive.hpp"
#include "types.hpp"
#include "symbol.hpp"
#include "section.hpp"

#include <string>
#include <unordered_map>
#include <list>

class Assembler
{
private:
  std::unordered_map<std::string, Symbol *> symbol_table;
  std::unordered_map<std::string, Section *> section_table;

  Section *curr_section;

  bool is_first_round = false;
  bool parsing_error = false;

  // TODO: add object file field and (maybe?) ObjectFile class

  Assembler() {}
  Assembler(const Assembler &) = delete;
  Assembler &operator=(const Assembler &) = delete;
  ~Assembler();

public:
  static Assembler &get_instance();

  bool run();
  void add_symbol(Symbol *);
  void add_section(std::string section_name);
  void add_command(Command *);

  void parse_error(std::string err_msg);
};

#endif