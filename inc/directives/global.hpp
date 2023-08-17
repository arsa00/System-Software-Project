#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "../directive.hpp"
#include "../symbol.hpp"
#include <iostream>

namespace directive {

  class GLOBAL : public Directive {

    void execute() const override
    {
      std::cout << "-----------------------------" << std::endl << "GLOBAL DIRECTIVE EXECUTED. Symbol list: " << std::endl;

      for(Parameter* const& param : this->params) 
      {
        Symbol *symbol = (Symbol *)param;
        std::cout << symbol->get_name() << std::endl;
      }

      std::cout << "-----------------------------" << std::endl;
    }

  };

}

#endif