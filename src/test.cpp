#include "../inc/all_instructions.hpp"
#include "../inc/all_directives.hpp"
#include "../inc/symbol.hpp"
#include "../inc/literal.hpp"
#include "../inc/instruction.hpp"
#include <iostream>
#include "../inc/command_builder.hpp"


void test1()
{
  std::cout << std::endl << "----------- TEST 1 ----------- " << std::endl << std::endl;
  Symbol *symbol = new Symbol(".test");
  std::cout << symbol->get_name() << std::endl;

  Literal *literal = new Literal();
  literal->set_num_value(0x016);
  std::cout << literal->get_num_value() << std::endl;

  Instruction *ins = new instruction::HALT();
  ins->enque_param(symbol);
  ins->enque_param(literal);

  std::list<Parameter *> params = ins->get_params();

  while(!params.empty()) {
    Parameter *param = params.front();

    if(param->get_type() == type::SYMBOL) {
      std::cout << "PARAM [simbol]: " << ((Symbol *) param)->get_name() << std::endl;
    }
    else
      std::cout << "PARAM [literal]: " << ((Literal *) param)->get_num_value() << std::endl;

    params.pop_front();
  }

  ins->execute();

// test copy constructor and destructor for instructions
  instruction::HALT halt_ins = (instruction::HALT&)(*ins);
  Instruction *ins2 = new instruction::HALT(halt_ins);


  delete ins;

  ins2->execute();

  Command *ins3 = new instruction::HALT();
  ins3->set_params(ins2->get_params());
  ins3->enque_param(new Symbol(".test22"));

  params = ins3->get_params();

  while(!params.empty()) {
    Parameter *param = params.front();

    if(param->get_type() == type::SYMBOL) {
      std::cout << "PARAM [simbol]: " << ((Symbol *) param)->get_name() << std::endl;
    }
    else
      std::cout << "PARAM [literal]: " << ((Literal *) param)->get_num_value() << std::endl;

    params.pop_front();
  }
}

void test2()
{
  std::cout << std::endl << "----------- TEST 2 ----------- " << std::endl << std::endl;
  Symbol *symbol = new Symbol(".test");

  Literal *literal = new Literal(1212);
  Literal *literal2 = new Literal(15190310);
  Literal *literal3 = new Literal("Proba tekst literala");

  CommandBuilder::get_instance().enque_param(symbol);
  CommandBuilder::get_instance().enque_param(literal);
  CommandBuilder::get_instance().enque_param(literal2);
  CommandBuilder::get_instance().enque_param(literal3);

  Instruction *ins = CommandBuilder::get_instance().build_instruction(type::HALT);

  std::list<Parameter *> params = ins->get_params();

  while(!params.empty()) {
    Parameter *param = params.front();

    if(param->get_type() == type::SYMBOL) {
      std::cout << "PARAM [simbol]: " << ((Symbol *) param)->get_name() << std::endl;
    }
    else // TODO: see how will client know which data is set (maybe two separated classes NUMERIC_LITERAL && STRING_LITERAL?)
      std::cout << "PARAM [literal]: " << ((Literal *) param)->get_num_value()  << " : " << ((Literal *) param)->get_str_value() << std::endl;

    params.pop_front();
  }

  ins->execute();
  delete ins;

  // CommandBuilder::get_instance().enque_param(symbol);
  CommandBuilder::get_instance().enque_param(new Symbol("dvrednost"));
  CommandBuilder::get_instance().enque_param(new Symbol("test_builder"));
  CommandBuilder::get_instance().enque_param(new Symbol(".working"));
  Directive *dir = CommandBuilder::get_instance().build_directive(type::GLOBAL);
  dir->execute();
  delete dir;

  ins = CommandBuilder::get_instance().build_instruction(type::INT);
  ins->execute();
  delete ins;

  CommandBuilder::get_instance().enque_param(new Symbol(".extern.text"));
  CommandBuilder::get_instance().enque_param(new Symbol("_working_"));
  dir = CommandBuilder::get_instance().build_directive(type::EXTERN);
  dir->execute();
  delete dir;
}