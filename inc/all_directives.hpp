#ifndef ALL_DIRECTIVES_HPP
#define ALL_DIRECTIVES_HPP

#include "directive.hpp"

namespace directive {

  class GLOBAL : public Directive {
    void execute() const override;
  };
  class EXTERN : public Directive {
    void execute() const override;
  };
  class SECTION : public Directive {
    void execute() const override;
  };
  class WORD : public Directive {
    void execute() const override;
  };
  class SKIP : public Directive {
    void execute() const override;
  };
  class ASCII : public Directive {
    void execute() const override;
  };
  // class EQU : public Directive {
  //   void execute() const override;
  // };
  class END : public Directive {
    void execute() const override;
  };

}

#endif