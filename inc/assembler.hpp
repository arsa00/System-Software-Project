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
  Section *no_data_section;

  bool is_first_round = false;
  bool parsing_err = false;
  bool internal_err = false;
  int symbol_id = 0;

  // TODO: add object file field and (maybe?) ObjectFile class

  Assembler();
  Assembler(const Assembler &) = delete;
  Assembler &operator=(const Assembler &) = delete;
  ~Assembler();

  void set_id_to_sym(Parameter *param);

public:
  static Assembler &get_instance();

  bool run();

  // create and define symbol if symbol is not alredy created
  // if it is created, set section and value (and defined flag) ==> define symbol
  Symbol *add_symbol(std::string name);
  Symbol *add_symbol(char *name);

  // create/declare not-defined symbol
  Symbol *create_symbol(std::string name);
  Symbol *create_symbol(char *name);

  void add_section(std::string section_name);
  void add_command(Command *cmd);

  void parse_error(std::string err_msg);
  void internal_error(std::string err_msg);
};

#endif