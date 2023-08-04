#ifndef TYPES_HPP
#define TYPES_HPP

namespace type {

  enum PARAMETER_TYPE { SYMBOL = 0, LITERAL };
  enum COMMAND_TYPE { INSTRUCTION = 0, DIRECTIVE };

  enum INSTRUCTION_TYPE
  {
    HALT,   INT,  IRET, CALL, RET,
    JMP,    BEQ,  BNE,  BGT,  PUSH,
    POP,    XCHG, ADD,  SUB,  MUL,
    DIV,    NOT,  AND,  OR,   XOR,
    SHL,    SHR,  LD,   ST,   CSRRD,
    CSRWR
  };

  enum DIRECTIVE_TYPE
  {
    GLOBAL, EXTERN, SECTION,  WORD,
    SKIP,   ASCII,  EQU,      END
  };

  enum MEMORY_ADDRESSING_MODES 
  {
    IMMED, MEM_DIR, REG_DIR, REG_IND, REG_IND_WITH_DISP
  };

}

#endif