#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include "instruction.hpp"
#include "directive.hpp"
#include "types.hpp"

class Assembler
{
private:
  Assembler() {}
  Assembler(const Assembler &) = delete;
  Assembler &operator=(const Assembler &) = delete;

public:
  static Assembler &get_instance();
};

#endif