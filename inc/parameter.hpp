#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include "types.hpp"

class Parameter
{
private:
  type::PARAMETER_TYPE type;
  int id = -1;

public:
  Parameter(type::PARAMETER_TYPE type) : type(type) {}

  virtual Parameter *clone() const;
  type::PARAMETER_TYPE get_type() const { return type; }

  // this methods are not supported for literals, therefor are virtual
  virtual bool set_id(int id);
  virtual int get_id();
};

#endif