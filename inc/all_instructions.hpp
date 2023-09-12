#ifndef ALL_INSTRUCTIONS_HPP
#define ALL_INSTRUCTIONS_HPP

#include "instruction.hpp"

namespace instruction
{

  class HALT : public Instruction
  {
  public:
    HALT();
    void execute(Section *dest_section) const override;
  };

  class INT : public Instruction
  {
  public:
    INT();
    void execute(Section *dest_section) const override;
  };

  class IRET : public Instruction
  {
  public:
    IRET();
    void execute(Section *dest_section) const override;
  };

  class CALL : public Instruction
  {
  public:
    CALL();
    void execute(Section *dest_section) const override;
  };

  class RET : public Instruction
  {
  public:
    RET();
    void execute(Section *dest_section) const override;
  };

  class JMP : public Instruction
  {
  public:
    JMP();
    void execute(Section *dest_section) const override;
  };

  class BEQ : public Instruction
  {
  public:
    BEQ();
    void execute(Section *dest_section) const override;
  };

  class BNE : public Instruction
  {
  public:
    BNE();
    void execute(Section *dest_section) const override;
  };

  class BGT : public Instruction
  {
  public:
    BGT();
    void execute(Section *dest_section) const override;
  };

  class PUSH : public Instruction
  {
  public:
    PUSH();
    void execute(Section *dest_section) const override;
  };

  class POP : public Instruction
  {
  public:
    POP();
    void execute(Section *dest_section) const override;
  };

  class XCHG : public Instruction
  {
  public:
    XCHG();
    void execute(Section *dest_section) const override;
  };

  class ADD : public Instruction
  {
  public:
    ADD();
    void execute(Section *dest_section) const override;
  };

  class SUB : public Instruction
  {
  public:
    SUB();
    void execute(Section *dest_section) const override;
  };

  class MUL : public Instruction
  {
  public:
    MUL();
    void execute(Section *dest_section) const override;
  };

  class DIV : public Instruction
  {
  public:
    DIV();
    void execute(Section *dest_section) const override;
  };

  class NOT : public Instruction
  {
  public:
    NOT();
    void execute(Section *dest_section) const override;
  };

  class AND : public Instruction
  {
  public:
    AND();
    void execute(Section *dest_section) const override;
  };

  class OR : public Instruction
  {
  public:
    OR();
    void execute(Section *dest_section) const override;
  };

  class XOR : public Instruction
  {
  public:
    XOR();
    void execute(Section *dest_section) const override;
  };

  class SHL : public Instruction
  {
  public:
    SHL();
    void execute(Section *dest_section) const override;
  };

  class SHR : public Instruction
  {
  public:
    SHR();
    void execute(Section *dest_section) const override;
  };

  class LD : public Instruction
  {
  public:
    LD();
    void execute(Section *dest_section) const override;
    uint32_t get_size() const override;
  };

  class ST : public Instruction
  {
  public:
    ST();
    void execute(Section *dest_section) const override;
  };

  class CSRRD : public Instruction
  {
  public:
    CSRRD();
    void execute(Section *dest_section) const override;
  };

  class CSRWR : public Instruction
  {
  public:
    CSRWR();
    void execute(Section *dest_section) const override;
  };

}

#endif