#include "../inc/all_instructions.hpp"
#include "../inc/symbol.hpp"
#include "../inc/literal.hpp"
#include "../auxiliary/inc/converters.hpp"
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
  this->size = 12;
  this->is_generating_data = true;
}

void instruction::IRET::execute(Section *dest_section) const
{
  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;

  // sp = sp + 8; ==> move sp 2 places backwards
  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_1);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::SP), static_cast<type::byte>(type::GP_REG::SP));

  displacement = converter::disp_to_byte_arr(8); // no need to call converter::write_to_upper_byte_half(0x00, displacement[0]);
                                                 // because higher 4bits of displacement[0] are already set to 0
  ins_bytes[2] = displacement[0];
  ins_bytes[3] = displacement[1];
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});

  // pop status first:
  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_6);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::CS_REG::STATUS_REG), static_cast<type::byte>(type::GP_REG::SP));

  displacement = converter::disp_to_byte_arr(-4); // no need to call converter::write_to_upper_byte_half(0x00, displacement[0]);
                                                  // because higher 4bits of displacement[0] are already set to 0
  ins_bytes[2] = displacement[0];
  ins_bytes[3] = displacement[1];
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});

  // pop pc:
  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_2);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::PC), static_cast<type::byte>(type::GP_REG::SP));

  displacement = converter::disp_to_byte_arr(-8); // no need to call converter::write_to_upper_byte_half(0x00, displacement[0]);
                                                  // because higher 4bits of displacement[0] are already set to 0
  ins_bytes[2] = displacement[0];
  ins_bytes[3] = displacement[1];
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
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
