#include "../inc/instructions/halt.hpp"
#include "../inc/symbol.hpp"
#include "../inc/literal.hpp"
#include "../inc/instruction.hpp"
#include <iostream>


void test1()
{
  std::cout << std::endl << "----------- TEST 1 ----------- " << std::endl << std::endl;
  Symbol *symbol = new Symbol(".test");
  std::cout << symbol->get_name() << std::endl;

  Literal *literal = new Literal();
  literal->set_num_value(0x016);
  std::cout << literal->get_num_value() << std::endl;

  Instruction *ins = new instruction::HALT();
  ins->enque_param(*symbol);
  ins->enque_param(*literal);

  std::list<Parameter> params = ins->get_params();

  while(!params.empty()) {
    Parameter param = params.back();

    if(param.get_type() == type::SYMBOL) {
      std::cout << "PARAM: " << ((Symbol) param).get_name(); << std::endl;
    }
    else
      std::cout << "PARAM: " << ((Literal) param).get_num_value(); << std::endl;

    params.pop_back();
  }

  ins->execute();
}