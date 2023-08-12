#ifndef HALT_HPP
#define HALT_HPP

#include "../instruction.hpp"
#include <iostream>

namespace instruction {

  class HALT : public Instruction {

    void execute() const override
    {
      std::cout << "HALT INSTRUCTION EXECUTED: 000000000" << std::endl;
    }

  };

}

#endif