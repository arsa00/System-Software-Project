#include "../inc/all_instructions.hpp"
#include "../inc/all_directives.hpp"
#include "../inc/symbol.hpp"
#include "../inc/literal.hpp"
#include "../inc/instruction.hpp"
#include <iostream>
#include "../inc/command_builder.hpp"
#include "../inc/section.hpp"
#include "../inc/assembler.hpp"
#include <vector>
#include "../auxiliary/inc/symbol_json.hpp"

void test1()
{
  std::cout << std::endl
            << "----------- TEST 1 ----------- " << std::endl
            << std::endl;

  Symbol *symbol = new Symbol(".test");
  std::cout << symbol->get_name() << std::endl;

  Literal *literal = new Literal();
  literal->set_num_value(0x016);
  std::cout << literal->get_num_value() << std::endl;

  Instruction *ins = new instruction::HALT();
  ins->enque_param(symbol);
  ins->enque_param(literal);

  std::list<Parameter *> params = ins->get_params();

  while (!params.empty())
  {
    Parameter *param = params.front();

    if (param->get_type() == type::PARAMETER_TYPE::SYMBOL)
    {
      std::cout << "PARAM [simbol]: " << ((Symbol *)param)->get_name() << std::endl;
    }
    else
      std::cout << "PARAM [literal]: " << ((Literal *)param)->get_num_value() << std::endl;

    params.pop_front();
  }

  ins->execute(nullptr);

  // test copy constructor and destructor for instructions
  instruction::HALT halt_ins = (instruction::HALT &)(*ins);
  Instruction *ins2 = new instruction::HALT(halt_ins);

  delete ins;

  ins2->execute(nullptr);

  Command *ins3 = new instruction::HALT();
  ins3->set_params(ins2->get_params());
  ins3->enque_param(new Symbol(".test22"));

  params = ins3->get_params();

  while (!params.empty())
  {
    Parameter *param = params.front();

    if (param->get_type() == type::PARAMETER_TYPE::SYMBOL)
    {
      std::cout << "PARAM [simbol]: " << ((Symbol *)param)->get_name() << std::endl;
    }
    else
      std::cout << "PARAM [literal]: " << ((Literal *)param)->get_num_value() << std::endl;

    params.pop_front();
  }
}

void test2()
{
  std::cout << std::endl
            << "----------- TEST 2 ----------- " << std::endl
            << std::endl;

  Assembler::get_instance().run(); // FIXME: won't work because the yyin is not defined!

  Symbol *symbol = new Symbol(".test");

  Literal *literal = new Literal(1212);
  Literal *literal2 = new Literal(15190310);
  Literal *literal3 = new Literal("Proba tekst literala");

  CommandBuilder::get_instance().enque_param(symbol);
  CommandBuilder::get_instance().enque_param(literal);
  CommandBuilder::get_instance().enque_param(literal2);
  CommandBuilder::get_instance().enque_param(literal3);

  Instruction *ins = CommandBuilder::get_instance().build_instruction(type::INSTRUCTION_TYPE::HALT);

  std::list<Parameter *> params = ins->get_params();

  while (!params.empty())
  {
    Parameter *param = params.front();

    if (param->get_type() == type::PARAMETER_TYPE::SYMBOL)
    {
      std::cout << "PARAM [simbol]: " << ((Symbol *)param)->get_name() << std::endl;
    }
    else // TODO: see how will client know which data is set (maybe two separated classes NUMERIC_LITERAL && STRING_LITERAL?)
      std::cout << "PARAM [literal]: " << ((Literal *)param)->get_num_value() << " : " << ((Literal *)param)->get_str_value() << std::endl;

    params.pop_front();
  }

  ins->execute(nullptr);
  delete ins;

  // CommandBuilder::get_instance().enque_param(symbol);
  CommandBuilder::get_instance().enque_param(new Symbol("dvrednost"));
  CommandBuilder::get_instance().enque_param(new Symbol("test_builder"));
  CommandBuilder::get_instance().enque_param(new Symbol(".working"));
  Directive *dir = CommandBuilder::get_instance().build_directive(type::DIRECTIVE_TYPE::GLOBAL);
  dir->execute(nullptr);
  delete dir;

  ins = CommandBuilder::get_instance().build_instruction(type::INSTRUCTION_TYPE::INT);
  ins->execute(nullptr);
  delete ins;

  CommandBuilder::get_instance().enque_param(new Symbol(".extern.text"));
  CommandBuilder::get_instance().enque_param(new Symbol("_working_"));
  dir = CommandBuilder::get_instance().build_directive(type::DIRECTIVE_TYPE::GLOBAL);
  dir->execute(nullptr);
  delete dir;
}

void test3()
{
  std::cout << std::endl
            << "----------- TEST 3 ----------- " << std::endl
            << std::endl;

  Section *section = new Section(".test3");
  section->set_id(0);
  section->print_output_file();

  Section *section2 = new Section(".test2");
  section2->set_id(1);

  Symbol *test_sym = new Symbol(".test");
  test_sym->set_id(3);
  test_sym->set_value(0x022);
  test_sym->set_section(section2);
  test_sym->set_defined_flag(true);
  // test_sym->set_global_flag(true);

  Literal *literal0 = new Literal(0xABCD);

  Command *halt_cmd = new instruction::HALT();
  Command *int_cmd = new instruction::INT();
  Command *iret_cmd = new instruction::IRET();
  Command *call_cmd = new instruction::CALL();
  call_cmd->enque_param(test_sym);
  Command *ret_cmd = new instruction::RET();
  Command *jmp_cmd = new instruction::JMP();
  jmp_cmd->enque_param(test_sym);

  section->add_command(int_cmd);
  section->add_command(iret_cmd);
  section->add_command(call_cmd);
  section->add_command(jmp_cmd);
  section->add_command(ret_cmd);
  section->add_command(halt_cmd); // add new above this line, so halt is last one
  section->create_output_file();

  section->print_output_file();
  std::cout << std::dec << "Location counter = " << section->get_curr_loc_cnt() << std::endl;
}

void testSymbolJsonRecord1()
{
  SymbolJsonRecord *symJson = new SymbolJsonRecord();
  symJson->set_id(1);
  symJson->set_is_final(false);
  symJson->set_is_global(true);
  symJson->set_name("test1");
  symJson->set_section(3);
  symJson->set_type(type::PARAMETER_TYPE::SYMBOL);
  symJson->set_value(22);

  std::string jsonFile = symJson->convert_to_json();

  SymbolJsonRecord *sym1Json = new SymbolJsonRecord();
  sym1Json->convert_to_json();
  sym1Json->init_from_json(jsonFile);
  sym1Json->convert_to_json();

  Symbol *sym = new Symbol("symbolInitTest");
  sym->set_id(22);
  sym->set_final_flag(true);
  sym->set_global_flag(false);
  sym->set_value(32);

  SymbolJsonRecord *sym2Json = new SymbolJsonRecord();
  sym2Json->init_from_symbol(sym);
  sym2Json->convert_to_json();

  Section *sec_sym = new Section("sectionInitTest");
  sec_sym->set_id(19151003);

  SymbolJsonRecord *sym3Json = new SymbolJsonRecord();
  sym3Json->init_from_section_symbol(sec_sym);
  sym3Json->convert_to_json();
}