#include "../inc/all_instructions.hpp"
#include "../inc/symbol.hpp"
#include "../inc/literal.hpp"
#include <iostream>
#include <vector>

instruction::HALT::HALT()
{
  this->is_generating_data = true;
}

void instruction::HALT::execute(Section *dest_section) const
{
  std::vector<type::byte> ins_data = {0, 0, 0, 0};
  dest_section->write_byte_arr(ins_data);
}

instruction::INT::INT()
{
  this->is_generating_data = true;
}

void instruction::INT::execute(Section *dest_section) const
{
  std::vector<type::byte> ins_data = {0x10, 0, 0, 0};
  dest_section->write_byte_arr(ins_data);
}

instruction::IRET::IRET()
{
  this->size = 8;
  this->is_generating_data = true;
}

void instruction::IRET::execute(Section *dest_section) const
{ // TODO: create auxiliary functions for bitwise operations
  // pop pc:
  type::byte byte1 = 0b10010011; // OC=1001, MMMM = 0011
  type::byte byte2 = ((static_cast<type::byte>(type::GP_REG::PC) << 4) & 0xF0) | (static_cast<type::byte>(type::GP_REG::SP) & 0x0F);
  byte2 &= 0xFF;

  int temp = (-4) & 0x0FFF; // disp = -4
  type::byte byte3 = (temp >> 8) & 0x00FF;
  type::byte byte4 = temp & 0x00FF;
  dest_section->write_byte_arr({byte1, byte2, byte3, byte4});

  // pop status
  byte1 = 0b10010111; // OC=1001, MMMM = 0111
  byte2 = ((static_cast<type::byte>(type::CS_REG::STATUS_REG) << 4) & 0xF0) | (static_cast<type::byte>(type::GP_REG::SP) & 0x0F);
  byte2 &= 0xFF;
  // byte3 and byte4 are same (disp is same)
  dest_section->write_byte_arr({byte1, byte2, byte3, byte4});
}

instruction::CALL::CALL()
{ // TODO: implement constructor
}

void instruction::CALL::execute(Section *dest_section) const
{
  // TODO: implement CALL execute
}

instruction::RET::RET()
{ // TODO: implement constructor
}

void instruction::RET::execute(Section *dest_section) const
{
  // TODO: implement RET execute
}

instruction::JMP::JMP()
{ // TODO: implement constructor
}

void instruction::JMP::execute(Section *dest_section) const
{
  // TODO: implement JMP execute
}

instruction::BEQ::BEQ()
{ // TODO: implement constructor
}

void instruction::BEQ::execute(Section *dest_section) const
{
  // TODO: implement BEQ execute
}

instruction::BNE::BNE()
{ // TODO: implement constructor
}

void instruction::BNE::execute(Section *dest_section) const
{
  // TODO: implement BNE execute
}

instruction::BGT::BGT()
{ // TODO: implement constructor
}

void instruction::BGT::execute(Section *dest_section) const
{
  // TODO: implement BGT execute
}

instruction::PUSH::PUSH()
{ // TODO: implement constructor
}

void instruction::PUSH::execute(Section *dest_section) const
{
  // TODO: implement PUSH execute
}

instruction::POP::POP()
{ // TODO: implement constructor
}

void instruction::POP::execute(Section *dest_section) const
{
  // TODO: implement POP execute
}

instruction::XCHG::XCHG()
{ // TODO: implement constructor
}

void instruction::XCHG::execute(Section *dest_section) const
{
  // TODO: implement XCHG execute
}

instruction::ADD::ADD()
{ // TODO: implement constructor
}

void instruction::ADD::execute(Section *dest_section) const
{
  // TODO: implement ADD execute
}

instruction::SUB::SUB()
{ // TODO: implement constructor
}

void instruction::SUB::execute(Section *dest_section) const
{
  // TODO: implement SUB execute
}

instruction::MUL::MUL()
{ // TODO: implement constructor
}

void instruction::MUL::execute(Section *dest_section) const
{
  // TODO: implement MUL execute
}

instruction::DIV::DIV()
{ // TODO: implement constructor
}

void instruction::DIV::execute(Section *dest_section) const
{
  // TODO: implement DIV execute
}

instruction::NOT::NOT()
{ // TODO: implement constructor
}

void instruction::NOT::execute(Section *dest_section) const
{
  // TODO: implement NOT execute
}

instruction::AND::AND()
{ // TODO: implement constructor
}

void instruction::AND::execute(Section *dest_section) const
{
  // TODO: implement AND execute
}

instruction::OR::OR()
{ // TODO: implement constructor
}

void instruction::OR::execute(Section *dest_section) const
{
  // TODO: implement OR execute
}

instruction::XOR::XOR()
{ // TODO: implement constructor
}

void instruction::XOR::execute(Section *dest_section) const
{
  // TODO: implement XOR execute
}

instruction::SHL::SHL()
{ // TODO: implement constructor
}

void instruction::SHL::execute(Section *dest_section) const
{
  // TODO: implement SHL execute
}

instruction::SHR::SHR()
{ // TODO: implement constructor
}

void instruction::SHR::execute(Section *dest_section) const
{
  // TODO: implement SHR execute
}

instruction::LD::LD()
{ // TODO: implement constructor
}

void instruction::LD::execute(Section *dest_section) const
{
  // TODO: implement LD execute
}

instruction::ST::ST()
{ // TODO: implement constructor
}

void instruction::ST::execute(Section *dest_section) const
{
  // TODO: implement ST execute
}

instruction::CSRRD::CSRRD()
{ // TODO: implement constructor
}

void instruction::CSRRD::execute(Section *dest_section) const
{
  // TODO: implement CSRRD execute
}

instruction::CSRWR::CSRWR()
{ // TODO: implement constructor
}

void instruction::CSRWR::execute(Section *dest_section) const
{
  // TODO: implement CSRWR execute
}
