#ifndef LITERAL_HPP
#define LITERAL_HPP

#include "parameter.hpp"
#include <string>

class Literal : public Parameter
{
private:
  uint32_t num_value;
  std::string str_value;

public:
  Literal();
  Literal(uint32_t val);
  Literal(std::string val);
  Literal(char *val);

  uint32_t get_num_value() const;
  std::string get_str_value() const;

  void set_num_value(uint32_t val);
  void set_str_value(std::string val);
  void set_str_value(char *val);

  Literal *clone() const override;
  bool set_id(int32_t id);
  int32_t get_id();
};

#endif