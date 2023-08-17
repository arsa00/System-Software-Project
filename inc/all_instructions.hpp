#ifndef ALL_INSTRUCTIONS_HPP
#define ALL_INSTRUCTIONS_HPP

#include "instruction.hpp"

namespace instruction {

  class HALT : public Instruction {
    public:
      void execute() const override;
  };
  class INT : public Instruction {
    public:
      void execute() const override;
  };
  class IRET : public Instruction {
    public:
      void execute() const override;
  };
  class CALL : public Instruction {
    public:
      void execute() const override;
  };
  class RET : public Instruction {
    public:
      void execute() const override;
  };
  class JMP : public Instruction {
    public:
      void execute() const override;
  };
  class BEQ : public Instruction {
    public:
      void execute() const override;
  };
  class BNE : public Instruction {
    public:
      void execute() const override;
  };
  class BGT : public Instruction {
    public:
      void execute() const override;
  };
  class PUSH : public Instruction {
    public:
      void execute() const override;
  };
  class POP : public Instruction {
    public:
      void execute() const override;
  };
  class XCHG : public Instruction {
    public:
      void execute() const override;
  };
  class ADD : public Instruction {
    public:
      void execute() const override;
  };
  class SUB : public Instruction {
    public:
      void execute() const override;
  };
  class MUL : public Instruction {
    public:
      void execute() const override;
  };
  class DIV : public Instruction {
    public:
      void execute() const override;
  };
  class NOT : public Instruction {
    public:
      void execute() const override;
  };
  class AND : public Instruction {
    public:
      void execute() const override;
  };
  class OR : public Instruction {
    public:
      void execute() const override;
  };
  class XOR : public Instruction {
    public:
      void execute() const override;
  };
  class SHL : public Instruction {
    public:
      void execute() const override;
  };
  class SHR : public Instruction {
    public:
      void execute() const override;
  };
  class LD : public Instruction {
    public:
      void execute() const override;
  };
  class ST : public Instruction {
    public:
      void execute() const override;
  };
  class CSRRD : public Instruction {
    public:
      void execute() const override;
  };
  class CSRWR : public Instruction {
    public:
      void execute() const override;
  };

}

#endif