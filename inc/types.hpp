#ifndef TYPES_HPP
#define TYPES_HPP

namespace type
{

  using byte = unsigned char;

  enum class PARAMETER_TYPE
  {
    SYMBOL = 0,
    LITERAL,
    SECTION
  };
  enum class COMMAND_TYPE
  {
    INSTRUCTION = 0,
    DIRECTIVE
  };

  enum class INSTRUCTION_TYPE
  {
    HALT,
    INT,
    IRET,
    CALL,
    RET,
    JMP,
    BEQ,
    BNE,
    BGT,
    PUSH,
    POP,
    XCHG,
    ADD,
    SUB,
    MUL,
    DIV,
    NOT,
    AND,
    OR,
    XOR,
    SHL,
    SHR,
    LD,
    ST,
    CSRRD,
    CSRWR
  };

  enum class DIRECTIVE_TYPE
  {
    GLOBAL,
    EXTERN,
    SECTION,
    WORD,
    SKIP,
    ASCII,
    EQU,
    END
  };

  enum class MEMORY_ADDRESSING_MODES
  {
    IMMED,
    MEM_DIR,
    REG_DIR,
    REG_IND,
    REG_IND_WITH_DISP
  };

  enum class GP_REG
  {
    R0 = 0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    SP,
    PC,
    NO_REG = -1
  };

  enum class CS_REG
  {
    STATUS_REG = 0,
    HANDLER_REG,
    CAUSE_REG,
    NO_REG = -1
  };
}

#endif