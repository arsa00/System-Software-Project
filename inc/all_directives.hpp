#ifndef ALL_DIRECTIVES_HPP
#define ALL_DIRECTIVES_HPP

#include "directive.hpp"
#include "literal.hpp"

namespace directive
{

  class GLOBAL : public Directive
  {
  public:
    GLOBAL();
    void execute(Section *dest_section) const override;
  };

  class EXTERN : public Directive
  {
  public:
    EXTERN();
    void execute(Section *dest_section) const override;
  };

  class SECTION : public Directive
  {
  public:
    SECTION();
    void execute(Section *dest_section) const override;
  };

  class WORD : public Directive
  {
  public:
    WORD();
    void execute(Section *dest_section) const override;
  };

  class SKIP : public Directive
  {
  public:
    SKIP();
    void execute(Section *dest_section) const override;
  };

  class ASCII : public Directive
  {
  public:
    ASCII();
    void execute(Section *dest_section) const override;
  };

  class END : public Directive
  {
  public:
    END();
    void execute(Section *dest_section) const override;
  };

  // class EQU : public Directive
  // {
  // public:
  //   EQU();
  //   void execute(Section *dest_section) const override;
  // };

}

#endif