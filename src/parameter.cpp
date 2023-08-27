#include "../inc/parameter.hpp"

Parameter *Parameter::clone() const
{
  return new Parameter(*this);
}

bool Parameter::set_id(int32_t id)
{
  this->id = id;
  return true;
}

int32_t Parameter::get_id()
{
  return this->id;
}