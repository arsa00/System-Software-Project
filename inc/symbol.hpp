#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include "parameter.hpp"
#include "section.hpp"
#include <string>

class Symbol : public Parameter
{
private:
  std::string name;
  uint32_t value;

  bool is_global = false;
  bool is_defined = false;
  bool is_final = false;  // indicates if symbol has final value
  bool has_value = false; // indicates if symbol has any value set
  Section *section = nullptr;

public:
  Symbol(std::string name);

  void set_section(Section *section);
  Section *get_section() const;

  void set_global_flag(bool global_flag);
  bool get_global_flag() const;

  void set_defined_flag(bool defined_flag);
  bool get_defined_flag() const;

  void set_final_flag(bool final_flag);
  bool get_final_flag() const;

  uint32_t get_value() const;
  void set_value(uint32_t val);
  bool has_set_value() const;

  void set_name(std::string val);
  void set_name(char *val);
  std::string get_name() const;

  Symbol *clone() const override;
};

#endif