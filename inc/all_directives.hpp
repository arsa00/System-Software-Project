#ifndef ALL_DIRECTIVES_HPP
#define ALL_DIRECTIVES_HPP

#include "directive.hpp"
#include "literal.hpp"

namespace directive {

  class GLOBAL : public Directive {
    public:
      void execute() const override;
  };
  class EXTERN : public Directive {
    public:
      void execute() const override;
  };
  class SECTION : public Directive {
    public:
      void execute() const override;
  };
  class WORD : public Directive {
    public:
      void execute() const override;
  };
  class SKIP : public Directive {
    public:
      void execute() const override;
  };
  class ASCII : public Directive {
    public:
      void execute() const override;
  };
  class END : public Directive {
    public:
      void execute() const override;
  };
  // class EQU : public Directive {
  //   public:
  //     void execute() const override;
  // };

}

#endif