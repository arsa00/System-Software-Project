#include "../inc/all_instructions.hpp"
#include "../inc/symbol.hpp"
#include "../inc/literal.hpp"
#include "../auxiliary/inc/converters.hpp"
#include "../inc/assembler.hpp"
#include "../inc/literal_pool_record.hpp"
#include "../inc/relocation_record.hpp"
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

  displacement = converter::disp_to_byte_arr(8); // no need to call converter::write_to_upper_byte_half(0x00, &displacement[0]);
                                                 // because higher 4bits of displacement[0] are already set to 0
  ins_bytes[2] = displacement[0];
  ins_bytes[3] = displacement[1];
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});

  // pop status first:
  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_6);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::CS_REG::STATUS_REG), static_cast<type::byte>(type::GP_REG::SP));

  displacement = converter::disp_to_byte_arr(-4); // no need to call converter::write_to_upper_byte_half(0x00, &displacement[0]);
                                                  // because higher 4bits of displacement[0] are already set to 0
  ins_bytes[2] = displacement[0];
  ins_bytes[3] = displacement[1];
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});

  // pop pc:
  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_2);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::PC), static_cast<type::byte>(type::GP_REG::SP));

  displacement = converter::disp_to_byte_arr(-8); // no need to call converter::write_to_upper_byte_half(0x00, &displacement[0]);
                                                  // because higher 4bits of displacement[0] are already set to 0
  ins_bytes[2] = displacement[0];
  ins_bytes[3] = displacement[1];
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
}

void create_jump_ins(Section *dest_section, Parameter *param, type::CPU_INSTRUCTIONS ins_direct, type::CPU_INSTRUCTIONS ins_indirect, uint32_t ins_size, type::GP_REG gprB, type::GP_REG gprC)
{
  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;

  if (param->get_type() == type::PARAMETER_TYPE::LITERAL)
  {
    // literal passed as parameter
    uint32_t value = ((Literal *)param)->get_num_value();
    if (value <= type::MAX_UNSIGNED_DISP)
    {
      // literal addr can fit in 12 bits
      ins_bytes[0] = static_cast<type::byte>(ins_direct);
      ins_bytes[1] = converter::create_byte_of_two_halves(0, static_cast<type::byte>(gprB));
      displacement = converter::disp_to_byte_arr(value);
      converter::write_to_upper_byte_half(static_cast<type::byte>(gprC), &displacement[0]);
      ins_bytes[2] = displacement[0];
      ins_bytes[3] = displacement[1];
      dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
    }
    else
    {
      // literal addr can't fit in 12 bits, write in pool literal and, later, read from mem address to jump on
      LiteralPoolRecord *literal_from_pool = new LiteralPoolRecord(value, false);
      dest_section->literal_pool_insert_new(literal_from_pool);

      ins_bytes[0] = static_cast<type::byte>(ins_indirect);
      ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::PC), static_cast<type::byte>(gprB));
      displacement = converter::disp_to_byte_arr(literal_from_pool->get_address() - dest_section->get_curr_loc_cnt() - ins_size);
      converter::write_to_upper_byte_half(static_cast<type::byte>(gprC), &displacement[0]);
      ins_bytes[2] = displacement[0];
      ins_bytes[3] = displacement[1];
      dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
    }
  }
  else if (param->get_type() == type::PARAMETER_TYPE::SYMBOL)
  {
    // symbol passed as parameter
    Symbol *sym = (Symbol *)param;

    if (!sym->get_defined_flag())
    {
      Assembler::get_instance().internal_error("Using undefined symbol within call instruction.");
      return;
    }

    if (!sym->has_set_value())
    {
      // extern symbol is used
      // get address to jump on from literal pool, and mark it (record in literal pool) as relocatable
      LiteralPoolRecord *literal_from_pool = new LiteralPoolRecord(sym->get_id(), true);
      dest_section->literal_pool_insert_new(literal_from_pool);

      // create relocation record for literal in pool, and add it to section's relocations list
      RelocationRecord *rel_record = new RelocationRecord(literal_from_pool->get_address(), sym->get_id(), type::RELOCATIONS::ABS_32U);
      dest_section->add_new_relocation(rel_record);

      ins_bytes[0] = static_cast<type::byte>(ins_indirect);
      ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::PC), static_cast<type::byte>(gprB));
      displacement = converter::disp_to_byte_arr(literal_from_pool->get_address() - dest_section->get_curr_loc_cnt() - ins_size);
      converter::write_to_upper_byte_half(static_cast<type::byte>(gprC), &displacement[0]);
      ins_bytes[2] = displacement[0];
      ins_bytes[3] = displacement[1];
      dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
    }
    else
    {
      if (sym->get_section()->get_id() == dest_section->get_id())
      {
        // symbol is in the same section
        int32_t disp_value = sym->get_value() - dest_section->get_curr_loc_cnt() - ins_size;
        if (disp_value < type::MAX_NEG_DISP || disp_value > type::MAX_POS_DISP)
        {
          Assembler::get_instance().internal_error("Destination symbol is too far away to be called from call instruction.");
          return;
        }

        ins_bytes[0] = static_cast<type::byte>(ins_direct);
        ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::PC), static_cast<type::byte>(gprB));
        displacement = converter::disp_to_byte_arr(disp_value);
        converter::write_to_upper_byte_half(static_cast<type::byte>(gprC), &displacement[0]);
        ins_bytes[2] = displacement[0];
        ins_bytes[3] = displacement[1];
        dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
      }
      else
      {
        // symbol is not in the same section
        // get address to jump on from literal pool, and mark it (record in literal pool) as relocatable
        LiteralPoolRecord *literal_from_pool = new LiteralPoolRecord(sym->get_id(), true);
        dest_section->literal_pool_insert_new(literal_from_pool);

        // create relocation record for literal in pool, and add it to section's relocations list
        int32_t sym_id = sym->get_global_flag() ? sym->get_id() : sym->get_section()->get_id();
        uint32_t addend = sym->get_global_flag() ? 0 : sym->get_value();
        RelocationRecord *rel_record = new RelocationRecord(literal_from_pool->get_address(), sym_id, type::RELOCATIONS::ABS_32U, addend);
        dest_section->add_new_relocation(rel_record);

        ins_bytes[0] = static_cast<type::byte>(ins_indirect);
        ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::PC), static_cast<type::byte>(gprB));
        displacement = converter::disp_to_byte_arr(literal_from_pool->get_address() - dest_section->get_curr_loc_cnt() - ins_size);
        converter::write_to_upper_byte_half(static_cast<type::byte>(gprC), &displacement[0]);
        ins_bytes[2] = displacement[0];
        ins_bytes[3] = displacement[1];
        dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
      }
    }
  }
  else
  {
    Assembler::get_instance().internal_error("Wrong parameter given to call instruction.");
    return;
  }
}

instruction::CALL::CALL()
{
  this->is_generating_data = true;
}

void instruction::CALL::execute(Section *dest_section) const
{
  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;
  std::list<Parameter *> params = this->get_params();

  if (params.empty())
  {
    Assembler::get_instance().internal_error("No parameter given to call instruction.");
    return;
  }

  if (params.size() > 1)
  {
    Assembler::get_instance().internal_error("Too parameters given to call instruction.");
    return;
  }

  Parameter *param = params.front();

  if (param->get_type() == type::PARAMETER_TYPE::LITERAL)
  {
    // literal passed as parameter
    uint32_t value = ((Literal *)param)->get_num_value();
    if (value <= type::MAX_UNSIGNED_DISP)
    {
      // literal addr can fit in 12 bits
      ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::CALL_0);
      ins_bytes[1] = 0;
      displacement = converter::disp_to_byte_arr(value);
      ins_bytes[2] = displacement[0];
      ins_bytes[3] = displacement[1];
      dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
    }
    else
    {
      // literal addr can't fit in 12 bits, write in pool literal and, later, read from mem address to jump on
      LiteralPoolRecord *literal_from_pool = new LiteralPoolRecord(value, false);
      dest_section->literal_pool_insert_new(literal_from_pool);

      ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::CALL_1);
      ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::PC), 0);
      displacement = converter::disp_to_byte_arr(literal_from_pool->get_address() - dest_section->get_curr_loc_cnt() - this->get_size());
      ins_bytes[2] = displacement[0];
      ins_bytes[3] = displacement[1];
      dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
    }
  }
  else if (param->get_type() == type::PARAMETER_TYPE::SYMBOL)
  {
    // symbol passed as parameter
    Symbol *sym = (Symbol *)param;

    if (!sym->get_defined_flag())
    {
      Assembler::get_instance().internal_error("Using undefined symbol within call instruction.");
      return;
    }

    if (!sym->has_set_value())
    {
      // extern symbol is used
      // get address to jump on from literal pool, and mark it (record in literal pool) as relocatable
      LiteralPoolRecord *literal_from_pool = new LiteralPoolRecord(0, true);
      dest_section->literal_pool_insert_new(literal_from_pool);

      // create relocation record for literal in pool, and add it to section's relocations list
      RelocationRecord *rel_record = new RelocationRecord(literal_from_pool->get_address(), sym->get_id(), type::RELOCATIONS::ABS_32U);
      dest_section->add_new_relocation(rel_record);

      ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::CALL_1);
      ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::PC), 0);
      displacement = converter::disp_to_byte_arr(literal_from_pool->get_address() - dest_section->get_curr_loc_cnt() - this->get_size());
      ins_bytes[2] = displacement[0];
      ins_bytes[3] = displacement[1];
      dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
    }
    else
    {
      if (sym->get_section()->get_id() == dest_section->get_id())
      {
        // symbol is in the same section
        int32_t disp_value = sym->get_value() - dest_section->get_curr_loc_cnt() - this->get_size();
        if (disp_value < type::MAX_NEG_DISP || disp_value > type::MAX_POS_DISP)
        {
          Assembler::get_instance().internal_error("Destination symbol is too far away to be called from call instruction.");
          return;
        }

        ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::CALL_0);
        ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::PC), 0);
        displacement = converter::disp_to_byte_arr(disp_value);
        ins_bytes[2] = displacement[0];
        ins_bytes[3] = displacement[1];
        dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
      }
      else
      {
        // symbol is not in the same section
        // get address to jump on from literal pool, and mark it (record in literal pool) as relocatable
        LiteralPoolRecord *literal_from_pool = new LiteralPoolRecord(0, true);
        dest_section->literal_pool_insert_new(literal_from_pool);

        // create relocation record for literal in pool, and add it to section's relocations list
        int32_t sym_id = sym->get_global_flag() ? sym->get_id() : sym->get_section()->get_id();
        uint32_t addend = sym->get_global_flag() ? 0 : sym->get_value();
        RelocationRecord *rel_record = new RelocationRecord(literal_from_pool->get_address(), sym_id, type::RELOCATIONS::ABS_32U, addend);
        dest_section->add_new_relocation(rel_record);

        ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::CALL_1);
        ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::PC), 0);
        displacement = converter::disp_to_byte_arr(literal_from_pool->get_address() - dest_section->get_curr_loc_cnt() - this->get_size());
        ins_bytes[2] = displacement[0];
        ins_bytes[3] = displacement[1];
        dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
      }
    }
  }
  else
  {
    Assembler::get_instance().internal_error("Wrong parameter given to call instruction.");
    return;
  }
}

instruction::RET::RET()
{
  this->is_generating_data = true;
}

void instruction::RET::execute(Section *dest_section) const
{
  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;

  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_3);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::PC), static_cast<type::byte>(type::GP_REG::SP));
  displacement = converter::disp_to_byte_arr(4);
  ins_bytes[2] = displacement[0];
  ins_bytes[3] = displacement[1];
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
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
