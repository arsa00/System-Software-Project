#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include "parameter.hpp"
#include <string>

class Symbol : public Parameter
{
private:
  std::string name;
  uint32_t value;

  // TODO: finish implementation
  bool is_global = false;
  // Section section; // TODO: add Section class

public:
  Symbol(std::string name, uint32_t value = -1) : Parameter(type::PARAMETER_TYPE::SYMBOL)
  {
    this->name = name;
    this->value = value;
  }

  uint32_t get_value() const;
  std::string get_name() const;

  void set_value(uint32_t val);
  void set_name(std::string val);
  void set_name(char *val);

  Symbol *clone() const override;
};

#endif