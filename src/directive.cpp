#include "../inc/directive.hpp"

void Directive::set_params(std::list<Parameter> params)
{
  this->params = params;
}

void Directive::enque_param(Parameter param) 
{
  this->params.push_back(param);
}

Parameter Directive::deque_param()
{
  Parameter param = this->params.back();
  this->params.pop_back();
  return param;
}

std::list<Parameter> Directive::get_params() const
{
  return this->params;
}