#include "../inc/all_instructions.hpp"
#include "../inc/symbol.hpp"
#include "../inc/literal.hpp"
#include <iostream>

void instruction::HALT::execute() const
{ // TODO: implement HALT execute
  std::cout << "-----------------------------" << std::endl;
  std::cout << "HALT INSTRUCTION EXECUTED" << std::endl;
  std::cout << "-----------------------------" << std::endl;
}

void instruction::INT::execute() const
{ // TODO: implement INT execute
  std::cout << "-----------------------------" << std::endl;
  std::cout << "INT INSTRUCTION EXECUTED" << std::endl;
  std::cout << "-----------------------------" << std::endl;
}

void instruction::IRET::execute() const 
{
  // TODO: implement IRET execute
}

void instruction::CALL::execute() const 
{
  // TODO: implement CALL execute
}

void instruction::RET::execute() const 
{
  // TODO: implement RET execute
}

void instruction::JMP::execute() const 
{
  // TODO: implement JMP execute
}

void instruction::BEQ::execute() const 
{
  // TODO: implement BEQ execute
}

void instruction::BNE::execute() const 
{
  // TODO: implement BNE execute
}

void instruction::BGT::execute() const 
{
  // TODO: implement BGT execute
}

void instruction::PUSH::execute() const 
{
  // TODO: implement PUSH execute
}

void instruction::POP::execute() const 
{
  // TODO: implement POP execute
}

void instruction::XCHG::execute() const 
{
  // TODO: implement XCHG execute
}

void instruction::ADD::execute() const 
{
  // TODO: implement ADD execute
}

void instruction::SUB::execute() const 
{
  // TODO: implement SUB execute
}

void instruction::MUL::execute() const 
{
  // TODO: implement MUL execute
}

void instruction::DIV::execute() const 
{
  // TODO: implement DIV execute
}

void instruction::NOT::execute() const 
{
  // TODO: implement NOT execute
}

void instruction::AND::execute() const 
{
  // TODO: implement AND execute
}

void instruction::OR::execute() const 
{
  // TODO: implement OR execute
}

void instruction::XOR::execute() const 
{
  // TODO: implement XOR execute
}

void instruction::SHL::execute() const 
{
  // TODO: implement SHL execute
}

void instruction::SHR::execute() const 
{
  // TODO: implement SHR execute
}

void instruction::LD::execute() const 
{
  // TODO: implement LD execute
}

void instruction::ST::execute() const 
{
  // TODO: implement ST execute
}

void instruction::CSRRD::execute() const 
{
  // TODO: implement CSRRD execute
}

void instruction::CSRWR::execute() const 
{
  // TODO: implement CSRWR execute
}
