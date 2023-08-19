#include "../inc/assembler.hpp"

Assembler &Assembler::get_instance()
{
  static Assembler assembler_instance;
  return assembler_instance;
}