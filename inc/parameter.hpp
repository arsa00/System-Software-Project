#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include "types.hpp"

class Parameter
{
private:
  type::PARAMETER_TYPE type;
  
public:
  Parameter(type::PARAMETER_TYPE type) : type(type) { }

  virtual Parameter* clone() const;
  type::PARAMETER_TYPE get_type() const { return type; }
};

#endif