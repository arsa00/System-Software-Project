#include "../inc/parameter.hpp"

Parameter *Parameter::clone() const
{
  return new Parameter(*this);
}

bool Parameter::set_id(int id)
{
  this->id = id;
  return true;
}

int Parameter::get_id()
{
  return this->id;
}