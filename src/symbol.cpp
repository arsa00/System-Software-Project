#include "../inc/symbol.hpp"

Symbol::Symbol(std::string name) : Parameter(type::PARAMETER_TYPE::SYMBOL)
{
  this->name = name;
}

void Symbol::set_section(Section *section)
{
  this->section = section;
}

Section *Symbol::get_section() const
{
  return this->section;
}

void Symbol::set_global_flag(bool global_flag)
{
  this->is_global = global_flag;
}

bool Symbol::get_global_flag() const
{
  return this->is_global;
}

void Symbol::set_defined_flag(bool defined_flag)
{
  this->is_defined = defined_flag;
}

bool Symbol::get_defined_flag() const
{
  return this->is_defined;
}

uint32_t Symbol::get_value() const
{
  return this->value;
}

void Symbol::set_value(uint32_t val)
{
  this->value = val;
  this->has_value = true;
}

bool Symbol::has_set_value() const
{
  return this->has_value;
}

void Symbol::set_name(std::string val)
{
  this->name = val;
}

void Symbol::set_name(char *val)
{
  this->name.assign(val);
}

std::string Symbol::get_name() const
{
  return this->name;
}

Symbol *Symbol::clone() const
{
  return new Symbol(*this);
}