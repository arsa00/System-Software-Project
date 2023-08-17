#ifndef ALL_INSTRUCTIONS_HPP
#define ALL_INSTRUCTIONS_HPP

#include "instruction.hpp"

namespace instruction {

  class HALT : public Instruction {
    void execute() const override;
  };
  class INT : public Instruction {
    void execute() const override;
  };
  class IRET : public Instruction {
    void execute() const override;
  };
  class CALL : public Instruction {
    void execute() const override;
  };
  class RET : public Instruction {
    void execute() const override;
  };
  class JMP : public Instruction {
    void execute() const override;
  };
  class BEQ : public Instruction {
    void execute() const override;
  };
  class BNE : public Instruction {
    void execute() const override;
  };
  class BGT : public Instruction {
    void execute() const override;
  };
  class PUSH : public Instruction {
    void execute() const override;
  };
  class POP : public Instruction {
    void execute() const override;
  };
  class XCHG : public Instruction {
    void execute() const override;
  };
  class ADD : public Instruction {
    void execute() const override;
  };
  class SUB : public Instruction {
    void execute() const override;
  };
  class MUL : public Instruction {
    void execute() const override;
  };
  class DIV : public Instruction {
    void execute() const override;
  };
  class NOT : public Instruction {
    void execute() const override;
  };
  class AND : public Instruction {
    void execute() const override;
  };
  class OR : public Instruction {
    void execute() const override;
  };
  class XOR : public Instruction {
    void execute() const override;
  };
  class SHL : public Instruction {
    void execute() const override;
  };
  class SHR : public Instruction {
    void execute() const override;
  };
  class LD : public Instruction {
    void execute() const override;
  };
  class ST : public Instruction {
    void execute() const override;
  };
  class CSRRD : public Instruction {
    void execute() const override;
  };
  class CSRWR : public Instruction {
    void execute() const override;
  };

}

#endif