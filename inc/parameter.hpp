#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include "types.hpp"

class Parameter
{
private:
  type::PARAMETER_TYPE type;
  int32_t id = -1;

public:
  Parameter(type::PARAMETER_TYPE type) : type(type) {}

  virtual Parameter *clone() const;
  type::PARAMETER_TYPE get_type() const { return type; }

  // these methods are not supported for literals, therefore are virtual
  virtual bool set_id(int32_t id);
  virtual int32_t get_id();
};

#endif