#ifndef CONVERTERS_HPP
#define CONVERTERS_HPP

#include "../../inc/types.hpp"
#include <string>

namespace converter
{

  std::string directive_type_to_string(type::DIRECTIVE_TYPE dir_alias);
  std::string instruction_type_to_string(type::INSTRUCTION_TYPE ins_alias);

}

#endif