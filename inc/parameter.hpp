#include "types.hpp"

class Parameter
{
private:
  type::PARAMETER_TYPE type;
public:
  Parameter(type::PARAMETER_TYPE type) : type(type) { }
};
