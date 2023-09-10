#ifndef TYPES_HPP
#define TYPES_HPP

#include <inttypes.h>

namespace type
{

  using byte = unsigned char;
  using instruction_size = uint32_t;
  using addr_size = uint32_t;

  const static int32_t MAX_NEG_DISP = -2048;      // -2^11
  const static int32_t MAX_POS_DISP = 2047;       // 2^11 - 1
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
    NO_MODE = -1,
    IMMED,
    MEM_DIR,
    REG_DIR,
    REG_IND,
    REG_IND_WITH_DISP
  };

  enum class GP_REG
  {
    NO_REG = -1,
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
    PC
  };

  enum class CS_REG
  {
    NO_REG = -1,
    STATUS_REG = 0,
    HANDLER_REG,
    CAUSE_REG
  };

  enum class RELOCATIONS
  {
    ABS_32S, // absolute value, 32bit long, signed
    ABS_32U  // absolute value, 32bit long, unsigned
  };

  // NOTE: after adding new type, add it also to converter::cpu_nstruction_type_to_string functon
  enum class CPU_INSTRUCTIONS
  {
    HALT = 0b00000000,       // stops CPU
    INT = 0b00010000,        // push status; push pc; cause<=4; status<=status&(~0x1); pc<=handle;
    CALL_0 = 0b00100000,     // push pc; pc<=gpr[A]+gpr[B]+D;
    CALL_1 = 0b00100001,     // push pc; pc<=mem32[gpr[A]+gpr[B]+D];
    JMP_0 = 0b00110000,      // pc<=gpr[A]+D;
    JMP_1 = 0b00110001,      // if (gpr[B] == gpr[C]) pc<=gpr[A]+D;
    JMP_2 = 0b00110010,      // if (gpr[B] != gpr[C]) pc<=gpr[A]+D;
    JMP_3 = 0b00110011,      // if (gpr[B] signed> gpr[C]) pc<=gpr[A]+D;
    JMP_4 = 0b00111000,      // pc<=mem32[gpr[A]+D];
    JMP_5 = 0b00111001,      // if (gpr[B] == gpr[C]) pc<=mem32[gpr[A]+D];
    JMP_6 = 0b00111010,      // if (gpr[B] != gpr[C]) pc<=mem32[gpr[A]+D];
    JMP_7 = 0b00111011,      // if (gpr[B] signed> gpr[C]) pc<=mem32[gpr[A]+D];
    XCHG = 0b01000000,       // temp<=gpr[B]; gpr[B]<=gpr[C]; gpr[C]<=temp; #atomic
    ARITH_OP_0 = 0b01010000, // gpr[A]<=gpr[B] + gpr[C];
    ARITH_OP_1 = 0b01010001, // gpr[A]<=gpr[B] - gpr[C];
    ARITH_OP_2 = 0b01010010, // gpr[A]<=gpr[B] * gpr[C];
    ARITH_OP_3 = 0b01010011, // gpr[A]<=gpr[B] / gpr[C];
    LOGIC_OP_0 = 0b01100000, // gpr[A]<=~gpr[B];
    LOGIC_OP_1 = 0b01100001, // gpr[A]<=gpr[B] & gpr[C];
    LOGIC_OP_2 = 0b01100010, // gpr[A]<=gpr[B] | gpr[C];
    LOGIC_OP_3 = 0b01100011, // gpr[A]<=gpr[B] ^ gpr[C];
    SHIFT_OP_0 = 0b01110000, // gpr[A]<=gpr[B] << gpr[C];
    SHIFT_OP_1 = 0b01110001, // gpr[A]<=gpr[B] >> gpr[C];
    ST_DATA_0 = 0b10000000,  // mem32[gpr[A]+gpr[B]+D]<=gpr[C];
    ST_DATA_1 = 0b10000010,  // mem32[mem32[gpr[A]+gpr[B]+D]]<=gpr[C];
    ST_DATA_2 = 0b10000001,  // gpr[A]<=gpr[A]+D; mem32[gpr[A]]<=gpr[C];
    LD_DATA_0 = 0b10010000,  // gpr[A] <= csr[B];
    LD_DATA_1 = 0b10010001,  // gpr[A] <= gpr[B]+D;
    LD_DATA_2 = 0b10010010,  // gpr[A] <= mem32[gpr[B]+gpr[C]+D];
    LD_DATA_3 = 0b10010011,  // gpr[A] <= mem32[gpr[B]]; gpr[B]<=gpr[B]+D;
    LD_DATA_4 = 0b10010100,  // csr[A] <= gpr[B];
    LD_DATA_5 = 0b10010101,  // csr[A] <= csr[B]|D;
    LD_DATA_6 = 0b10010110,  // csr[A] <= mem32[gpr[B]+gpr[C]+D];
    LD_DATA_7 = 0b10010111   // csr[A] <= mem32[gpr[B]]; gpr[B]<=gpr[B]+D;
  };
}

#endif