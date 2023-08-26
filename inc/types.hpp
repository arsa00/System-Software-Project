#ifndef TYPES_HPP
#define TYPES_HPP

#include <inttypes.h>

namespace type
{

  using byte = unsigned char;
  using instruction_size = uint32_t;
  using addr_size = uint32_t;

  const static int MAX_NEG_DISP = -2048;          // -2^11
  const static int MAX_POS_DISP = 2047;           // 2^11 - 1
  const static uint16_t MAX_UNSIGNED_DISP = 4095; // 2^12 - 1

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

  enum class CPU_INSTRUCTIONS
  {
    CALL_0 = 0b00100000,    // push pc; pc<=gpr[A]+gpr[B]+D;
    CALL_1 = 0b00100001,    // push pc; pc<=mem32[gpr[A]+gpr[B]+D];
    LD_DATA_0 = 0b10010000, // gpr[A] <= csr[B];
    LD_DATA_1 = 0b10010001, // gpr[A] <= gpr[B]+D;
    LD_DATA_2 = 0b10010010, // gpr[A] <= mem32[gpr[B]+gpr[C]+D];
    LD_DATA_3 = 0b10010011, // gpr[A] <= mem32[gpr[B]]; gpr[B]<=gpr[B]+D;
    LD_DATA_4 = 0b10010100, // csr[A] <= gpr[B];
    LD_DATA_5 = 0b10010101, // csr[A] <= csr[B]|D;
    LD_DATA_6 = 0b10010110, // csr[A] <= mem32[gpr[B]+gpr[C]+D];
    LD_DATA_7 = 0b10010111  // csr[A] <= mem32[gpr[B]]; gpr[B]<=gpr[B]+D;
  };
}

#endif