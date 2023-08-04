#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include "parameter.hpp"
#include <string>

class Symbol : public Parameter
{
private:
  std::string name;
  uint32_t value;
public:
  Symbol(std::string name, uint32_t value = -1) : Parameter(type::SYMBOL) 
  { 
    this->name = name; 
    value = value;
  }

  uint32_t    get_value() const;
  std::string get_name() const;

  void set_value(uint32_t val);
  void set_name(std::string val);
  void set_name(char *val);

  ~Symbol();
};

#endif