#include "../inc/parameter.hpp"

Parameter* Parameter::clone() const { return new Parameter(*this); }