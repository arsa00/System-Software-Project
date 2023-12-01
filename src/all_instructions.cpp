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
  std::vector<type::byte> ins_data = {static_cast<type::byte>(type::CPU_INSTRUCTIONS::HALT), 0, 0, 0};
  dest_section->write_byte_arr(ins_data);
}

instruction::INT::INT()
{
  this->is_generating_data = true;
}

void instruction::INT::execute(Section *dest_section) const
{
  std::vector<type::byte> ins_data = {static_cast<type::byte>(type::CPU_INSTRUCTIONS::INT), 0, 0, 0};
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
      Assembler::get_instance().internal_error("Using undefined symbol: \"" + sym->get_name() + std::string("\". Cannot generate call instruction."));
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
    Assembler::get_instance().internal_error("Too many parameters given to call instruction.");
    return;
  }

  Parameter *param = params.front();
  create_jump_ins(dest_section, param, type::CPU_INSTRUCTIONS::CALL_0, type::CPU_INSTRUCTIONS::CALL_1, this->get_size(), type::GP_REG::R0, type::GP_REG::R0);
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
{
  this->is_generating_data = true;
}

void instruction::JMP::execute(Section *dest_section) const
{
  // TODO: test JMP instruction
  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;
  std::list<Parameter *> params = this->get_params();

  if (params.empty())
  {
    Assembler::get_instance().internal_error("No parameter given to jmp instruction.");
    return;
  }

  if (params.size() > 1)
  {
    Assembler::get_instance().internal_error("Too many parameters given to jmp instruction.");
    return;
  }

  Parameter *param = params.front();
  create_jump_ins(dest_section, param, type::CPU_INSTRUCTIONS::JMP_0, type::CPU_INSTRUCTIONS::JMP_4, this->get_size(), type::GP_REG::R0, type::GP_REG::R0);
}

instruction::BEQ::BEQ()
{
  this->is_generating_data = true;
}

void instruction::BEQ::execute(Section *dest_section) const
{
  // TODO: test BEQ instruction
  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;
  std::list<Parameter *> params = this->get_params();

  if (params.empty())
  {
    Assembler::get_instance().internal_error("No parameter given to beq instruction.");
    return;
  }

  if (params.size() > 1)
  {
    Assembler::get_instance().internal_error("Too many parameters given to beq instruction.");
    return;
  }

  if (this->get_gp_reg_0() == type::GP_REG::NO_REG || this->get_gp_reg_1() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP registers must be set to execute beq instruction.");
    return;
  }

  // XXX: is validation if any of gp regs is set to special gp registers (PC or SP) needed?

  Parameter *param = params.front();
  create_jump_ins(dest_section, param, type::CPU_INSTRUCTIONS::JMP_1, type::CPU_INSTRUCTIONS::JMP_5, this->get_size(), this->get_gp_reg_0(), this->get_gp_reg_1());
}

instruction::BNE::BNE()
{
  this->is_generating_data = true;
}

void instruction::BNE::execute(Section *dest_section) const
{
  // TODO: test BNE instruction
  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;
  std::list<Parameter *> params = this->get_params();

  if (params.empty())
  {
    Assembler::get_instance().internal_error("No parameter given to bne instruction.");
    return;
  }

  if (params.size() > 1)
  {
    Assembler::get_instance().internal_error("Too many parameters given to bne instruction.");
    return;
  }

  if (this->get_gp_reg_0() == type::GP_REG::NO_REG || this->get_gp_reg_1() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP registers must be set to execute bne instruction.");
    return;
  }

  // XXX: is validation if any of gp regs is set to special gp registers (PC or SP) needed?

  Parameter *param = params.front();
  create_jump_ins(dest_section, param, type::CPU_INSTRUCTIONS::JMP_2, type::CPU_INSTRUCTIONS::JMP_6, this->get_size(), this->get_gp_reg_0(), this->get_gp_reg_1());
}

instruction::BGT::BGT()
{
  this->is_generating_data = true;
}

void instruction::BGT::execute(Section *dest_section) const
{
  // TODO: test BGT instruction
  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;
  std::list<Parameter *> params = this->get_params();

  if (params.empty())
  {
    Assembler::get_instance().internal_error("No parameter given to bgt instruction.");
    return;
  }

  if (params.size() > 1)
  {
    Assembler::get_instance().internal_error("Too many parameters given to bgt instruction.");
    return;
  }

  if (this->get_gp_reg_0() == type::GP_REG::NO_REG || this->get_gp_reg_1() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP registers must be set to execute bgt instruction.");
    return;
  }

  // XXX: is validation if any of gp regs is set to special gp registers (PC or SP) needed?

  Parameter *param = params.front();
  create_jump_ins(dest_section, param, type::CPU_INSTRUCTIONS::JMP_3, type::CPU_INSTRUCTIONS::JMP_7, this->get_size(), this->get_gp_reg_0(), this->get_gp_reg_1());
}

instruction::PUSH::PUSH()
{
  this->is_generating_data = true;
}

void instruction::PUSH::execute(Section *dest_section) const
{
  // TODO: test PUSH instruction
  // push %gpr ==> sp <= sp - 4; mem32[sp] <= gpr; [use gp_reg0]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP register must be set to execute push instruction.");
    return;
  }

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;

  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::ST_DATA_2);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::SP), 0);
  displacement = converter::disp_to_byte_arr(-4);
  converter::write_to_upper_byte_half(static_cast<type::byte>(this->get_gp_reg_0()), &displacement[0]);
  ins_bytes[2] = displacement[0];
  ins_bytes[3] = displacement[1];
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
}

instruction::POP::POP()
{
  this->is_generating_data = true;
}

void instruction::POP::execute(Section *dest_section) const
{
  // TODO: test POP instruction
  // pop %gpr ===> gpr <= mem32[sp]; sp <= sp + 4; [use gp_reg0]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP register must be set to execute pop instruction.");
    return;
  }

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;

  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_3);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), static_cast<type::byte>(type::GP_REG::SP));
  displacement = converter::disp_to_byte_arr(4);
  ins_bytes[2] = displacement[0];
  ins_bytes[3] = displacement[1];
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
}

instruction::XCHG::XCHG()
{
  this->is_generating_data = true;
}

void instruction::XCHG::execute(Section *dest_section) const
{
  // TODO: test XCHG instruction
  // xchg %gprS, %gprD ==> temp <= gprD; gprD <= gprS; gprS <= temp; [gprS = gp_reg_0 | gprD = gp_reg_1]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG || this->get_gp_reg_1() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP registers must be set to execute xchg instruction.");
    return;
  }

  // XXX: is validation if any of gp regs is set to special gp registers (PC or SP) needed?

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;

  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::XCHG);
  ins_bytes[1] = converter::create_byte_of_two_halves(0, static_cast<type::byte>(this->get_gp_reg_1()));
  ins_bytes[2] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), 0);
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
}

void create_operation_instruction(Section *dest_section, type::CPU_INSTRUCTIONS ins, type::GP_REG gprA, type::GP_REG gprB, type::GP_REG gprC = type::GP_REG::R0, int16_t disp = 0)
{
  if (gprA == type::GP_REG::NO_REG || gprB == type::GP_REG::NO_REG || gprC == type::GP_REG::NO_REG)
  {
    std::string msg = "GP registers have non-consistent values: \n{";
    msg += "section name: \"" + dest_section->get_name() + std::string("\",\n");
    msg += "cpu instruction: \"" + converter::cpu_instruction_type_to_string(ins) + std::string("\",\n");
    msg += "gprA: " + std::to_string(static_cast<int16_t>(gprA)) + std::string(",\n");
    msg += "gprB: " + std::to_string(static_cast<int16_t>(gprB)) + std::string(",\n");
    msg += "gprC: " + std::to_string(static_cast<int16_t>(gprC)) + std::string("\n}");
    Assembler::get_instance().internal_error(msg);
    return;
  }

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement = converter::disp_to_byte_arr(disp);

  if (gprC != type::GP_REG::R0)
    converter::write_to_upper_byte_half(static_cast<type::byte>(gprC), &displacement[0]);

  ins_bytes[0] = static_cast<type::byte>(ins);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(gprA), static_cast<type::byte>(gprB));
  ins_bytes[2] = displacement[0];
  ins_bytes[3] = displacement[1];
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
}

instruction::ADD::ADD()
{
  this->is_generating_data = true;
}

void instruction::ADD::execute(Section *dest_section) const
{
  // TODO: test ADD instruction
  // add %gprS, %gprD ==> gprD <= gprD + gprS; [gprS = gp_reg_0 | gprD = gp_reg_1]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG || this->get_gp_reg_1() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP registers must be set to execute add instruction.");
    return;
  }

  // XXX: is validation if any of gp regs is set to special gp registers (PC or SP) needed?

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;

  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::ARITH_OP_0);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_1()), static_cast<type::byte>(this->get_gp_reg_1()));
  ins_bytes[2] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), 0);
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});

  // create_operation_instruction(dest_section, type::CPU_INSTRUCTIONS::ARITH_OP_0, this->get_gp_reg_1(), this->get_gp_reg_1(), this->get_gp_reg_0()); // TODO: test this
}

instruction::SUB::SUB()
{
  this->is_generating_data = true;
}

void instruction::SUB::execute(Section *dest_section) const
{
  // TODO: test SUB instruction
  // sub %gprS, %gprD ==> gprD <= gprD - gprS; [gprS = gp_reg_0 | gprD = gp_reg_1]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG || this->get_gp_reg_1() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP registers must be set to execute sub instruction.");
    return;
  }

  // XXX: is validation if any of gp regs is set to special gp registers (PC or SP) needed?

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;

  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::ARITH_OP_1);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_1()), static_cast<type::byte>(this->get_gp_reg_1()));
  ins_bytes[2] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), 0);
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});

  // create_operation_instruction(dest_section, type::CPU_INSTRUCTIONS::ARITH_OP_1, this->get_gp_reg_1(), this->get_gp_reg_1(), this->get_gp_reg_0()); // TODO: test this
}

instruction::MUL::MUL()
{
  this->is_generating_data = true;
}

void instruction::MUL::execute(Section *dest_section) const
{
  // TODO: test MUL instruction
  // mul %gprS, %gprD ==> gprD <= gprD * gprS; [gprS = gp_reg_0 | gprD = gp_reg_1]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG || this->get_gp_reg_1() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP registers must be set to execute mul instruction.");
    return;
  }

  // XXX: is validation if any of gp regs is set to special gp registers (PC or SP) needed?

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;

  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::ARITH_OP_2);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_1()), static_cast<type::byte>(this->get_gp_reg_1()));
  ins_bytes[2] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), 0);
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});

  // create_operation_instruction(dest_section, type::CPU_INSTRUCTIONS::ARITH_OP_2, this->get_gp_reg_1(), this->get_gp_reg_1(), this->get_gp_reg_0()); // TODO: test this
}

instruction::DIV::DIV()
{
  this->is_generating_data = true;
}

void instruction::DIV::execute(Section *dest_section) const
{
  // TODO: test DIV instruction
  // div %gprS, %gprD ==> gprD <= gprD / gprS; [gprS = gp_reg_0 | gprD = gp_reg_1]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG || this->get_gp_reg_1() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP registers must be set to execute div instruction.");
    return;
  }

  // XXX: is validation if any of gp regs is set to special gp registers (PC or SP) needed?

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;

  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::ARITH_OP_3);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_1()), static_cast<type::byte>(this->get_gp_reg_1()));
  ins_bytes[2] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), 0);
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});

  // create_operation_instruction(dest_section, type::CPU_INSTRUCTIONS::ARITH_OP_3, this->get_gp_reg_1(), this->get_gp_reg_1(), this->get_gp_reg_0()); // TODO: test this
}

instruction::NOT::NOT()
{
  this->is_generating_data = true;
}

void instruction::NOT::execute(Section *dest_section) const
{
  // TODO: test NOT instruction
  // not %gpr ==> gpr <= ~gpr; [grp = gp_reg_0]

  if (this->get_gp_reg_0() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP register must be set to execute not instruction.");
    return;
  }

  // XXX: is validation if gp reg is set to special gp registers (PC or SP) needed?

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;

  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LOGIC_OP_0);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), static_cast<type::byte>(this->get_gp_reg_0()));
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});

  // create_operation_instruction(dest_section, type::CPU_INSTRUCTIONS::LOGIC_OP_0, this->get_gp_reg_0(), this->get_gp_reg_0()); // TODO: test this
}

instruction::AND::AND()
{
  this->is_generating_data = true;
}

void instruction::AND::execute(Section *dest_section) const
{
  // TODO: test AND instruction
  // and %gprS, %gprD ==> gprD <= gprD & gprS; [gprS = gp_reg_0 | gprD = gp_reg_1]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG || this->get_gp_reg_1() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP registers must be set to execute and instruction.");
    return;
  }

  // XXX: is validation if any of gp regs is set to special gp registers (PC or SP) needed?

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;

  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LOGIC_OP_1);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_1()), static_cast<type::byte>(this->get_gp_reg_1()));
  ins_bytes[2] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), 0);
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});

  // create_operation_instruction(dest_section, type::CPU_INSTRUCTIONS::LOGIC_OP_1, this->get_gp_reg_1(), this->get_gp_reg_1(), this->get_gp_reg_0()); // TODO: test this
}

instruction::OR::OR()
{
  this->is_generating_data = true;
}

void instruction::OR::execute(Section *dest_section) const
{
  // TODO: test OR instruction
  // or %gprS, %gprD ==> gprD <= gprD | gprS; [gprS = gp_reg_0 | gprD = gp_reg_1]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG || this->get_gp_reg_1() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP registers must be set to execute or instruction.");
    return;
  }

  // XXX: is validation if any of gp regs is set to special gp registers (PC or SP) needed?

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;

  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LOGIC_OP_2);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_1()), static_cast<type::byte>(this->get_gp_reg_1()));
  ins_bytes[2] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), 0);
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});

  // create_operation_instruction(dest_section, type::CPU_INSTRUCTIONS::LOGIC_OP_2, this->get_gp_reg_1(), this->get_gp_reg_1(), this->get_gp_reg_0()); // TODO: test this
}

instruction::XOR::XOR()
{
  this->is_generating_data = true;
}

void instruction::XOR::execute(Section *dest_section) const
{
  // TODO: test XOR instruction
  // xor %gprS, %gprD ==> gprD <= gprD ^ gprS; [gprS = gp_reg_0 | gprD = gp_reg_1]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG || this->get_gp_reg_1() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP registers must be set to execute xor instruction.");
    return;
  }

  // XXX: is validation if any of gp regs is set to special gp registers (PC or SP) needed?

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;

  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LOGIC_OP_3);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_1()), static_cast<type::byte>(this->get_gp_reg_1()));
  ins_bytes[2] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), 0);
  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});

  // create_operation_instruction(dest_section, type::CPU_INSTRUCTIONS::LOGIC_OP_3, this->get_gp_reg_1(), this->get_gp_reg_1(), this->get_gp_reg_0()); // TODO: test this
}

instruction::SHL::SHL()
{
  this->is_generating_data = true;
}

void instruction::SHL::execute(Section *dest_section) const
{
  // TODO: test SHL instruction
  // shl %gprS, %gprD ==> gprD <= gprD << gprS; [gprS = gp_reg_0 | gprD = gp_reg_1]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG || this->get_gp_reg_1() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP registers must be set to execute shl instruction.");
    return;
  }

  // XXX: is validation if any of gp regs is set to special gp registers (PC or SP) needed?

  create_operation_instruction(dest_section, type::CPU_INSTRUCTIONS::SHIFT_OP_0, this->get_gp_reg_1(), this->get_gp_reg_1(), this->get_gp_reg_0());
}

instruction::SHR::SHR()
{
  this->is_generating_data = true;
}

void instruction::SHR::execute(Section *dest_section) const
{
  // TODO: test SHR instruction
  // shr %gprS, %gprD ==> gprD <= gprD >> gprS; [gprS = gp_reg_0 | gprD = gp_reg_1]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG || this->get_gp_reg_1() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP registers must be set to execute shr instruction.");
    return;
  }

  // XXX: is validation if any of gp regs is set to special gp registers (PC or SP) needed?

  create_operation_instruction(dest_section, type::CPU_INSTRUCTIONS::SHIFT_OP_1, this->get_gp_reg_1(), this->get_gp_reg_1(), this->get_gp_reg_0());
}

instruction::LD::LD()
{
  this->is_generating_data = true;
}

// needed to calculate size in virtual method, because it depends on mem_addr_mode
uint32_t instruction::LD::get_size() const
{
  // only if mem_addr_mode is 'mem dir', ld generates two cpu instructions [= 8 bytes]
  // in every other case it generates only one cpu instruction [= 4 bytes], which is default value for instruction size
  return this->get_mem_addr_mode() == type::MEMORY_ADDRESSING_MODES::MEM_DIR ? 2 * this->size : this->size;
}

void instruction::LD::execute(Section *dest_section) const
{
  // TODO: test LD instruction
  // ld operand, %gpr ==> gpr <= operand; [gpr = grp_reg_0]  OR  [operand = gpr_reg_0 | gpr = gpr_reg_1]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP register must be set to execute ld instruction.");
    return;
  }

  // XXX: is validation if gp reg is set to special gp registers (PC or SP) needed?

  if (this->get_mem_addr_mode() == type::MEMORY_ADDRESSING_MODES::NO_MODE)
  {
    Assembler::get_instance().internal_error("Memory addressing mode must be set to execute ld instruction.");
    return;
  }

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;
  Parameter *param = this->params.empty() ? nullptr : this->params.front();

  switch (this->get_mem_addr_mode())
  {
  case type::MEMORY_ADDRESSING_MODES::IMMED:
  { // generates one instruction ==> 4B
    if (!param)
    {
      Assembler::get_instance().internal_error("Parameter is needed when immed mem addr mode is used with ld instruction.");
      return;
    }

    if (param->get_type() == type::PARAMETER_TYPE::SYMBOL)
    {
      Symbol *sym = (Symbol *)param;
      if (!sym->get_defined_flag())
      {
        Assembler::get_instance().internal_error("Using undefined symbol: \"" + sym->get_name() + std::string("\". Cannot generate ld instruction [immed mem addr mode]."));
        return;
      }

      // insert new relocation record, because final value of symbol is needed
      // TODO: only time this is potentially not needed is if symbol is defined with .equ directive [can be checked with symbol's get_final_flag method]
      LiteralPoolRecord *literal_from_pool = new LiteralPoolRecord(sym->get_id(), true);
      dest_section->literal_pool_insert_new(literal_from_pool);

      // create relocation record for literal in pool, and add it to section's relocations collection
      int32_t sym_id = sym->get_global_flag() ? sym->get_id() : sym->get_section()->get_id();
      uint32_t addend = sym->get_global_flag() ? 0 : sym->get_value();
      RelocationRecord *rel_record = new RelocationRecord(literal_from_pool->get_address(), sym_id, type::RELOCATIONS::ABS_32S, addend);
      dest_section->add_new_relocation(rel_record);

      ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_2);
      ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), static_cast<type::byte>(type::GP_REG::PC));
      displacement = converter::disp_to_byte_arr(literal_from_pool->get_address() - dest_section->get_curr_loc_cnt() - this->get_size());
      ins_bytes[2] = displacement[0];
      ins_bytes[3] = displacement[1];
      dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
    }
    else if (param->get_type() == type::PARAMETER_TYPE::LITERAL)
    {
      // literal passed as parameter (it is singed value, because it is not an addres)
      int32_t value = ((Literal *)param)->get_num_value();
      if (value >= type::MAX_NEG_DISP && value <= type::MAX_POS_DISP)
      {
        // literal value can fit in 12 bits
        ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_1);
        ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), static_cast<type::byte>(type::GP_REG::R0));
        displacement = converter::disp_to_byte_arr(value);
        ins_bytes[2] = displacement[0];
        ins_bytes[3] = displacement[1];
        dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
      }
      else
      {
        // literal value can't fit in 12 bits, write in pool literal and, later, read from it
        LiteralPoolRecord *literal_from_pool = new LiteralPoolRecord(value, false);
        dest_section->literal_pool_insert_new(literal_from_pool);

        ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_2);
        ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), static_cast<type::byte>(type::GP_REG::PC));
        displacement = converter::disp_to_byte_arr(literal_from_pool->get_address() - dest_section->get_curr_loc_cnt() - this->get_size());
        ins_bytes[2] = displacement[0];
        ins_bytes[3] = displacement[1];
        dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
      }
    }
    else
    {
      Assembler::get_instance().internal_error("Wrong parameter passed to ld instruction [immed mem addr mode].");
      return;
    }
  }
  break;

  case type::MEMORY_ADDRESSING_MODES::MEM_DIR:
  { // generates two instructions ==> 8B
    if (!param)
    {
      Assembler::get_instance().internal_error("Parameter is needed when mem dir addr mode is used with ld instruction.");
      return;
    }

    if (param->get_type() == type::PARAMETER_TYPE::SECTION)
    {
      Assembler::get_instance().internal_error("Wrong parameter passed to ld instruction [mem dir addr mode].");
      return;
    }

    // create temp ld instruction to load symbol/litreal value in passed GP register
    // temp_param is needed because Instruction's destructor deletes all literals in it's params list. Symbols are not deleted.
    Parameter *temp_param = param->get_type() == type::PARAMETER_TYPE::SYMBOL ? param : ((Literal *)param)->clone();
    instruction::LD *ld_temp = new instruction::LD();
    ld_temp->set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::IMMED);
    ld_temp->set_gp_reg_0(this->get_gp_reg_0());
    ld_temp->enque_param(temp_param);
    ld_temp->execute(dest_section); // this should generate instrcution that loads param's value in passed GPR
    delete ld_temp;

    // now we can assume that value of parameter is stored in passed GRP
    // create following instruction: gpr <= mem32[gpr]
    ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_2);
    ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), static_cast<type::byte>(this->get_gp_reg_0()));
    displacement = converter::disp_to_byte_arr(0);
    ins_bytes[2] = displacement[0];
    ins_bytes[3] = displacement[1];
    dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
  }
  break;

  case type::MEMORY_ADDRESSING_MODES::REG_DIR:
  { // generates one instruction ==> 4B
    if (this->get_gp_reg_1() == type::GP_REG::NO_REG)
    {
      Assembler::get_instance().internal_error("Both GP registers must be set when reg dir mem addr mode is used with ld instruction.");
      return;
    }

    ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_1);
    ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_1()), static_cast<type::byte>(this->get_gp_reg_0()));
    displacement = converter::disp_to_byte_arr(0);
    ins_bytes[2] = displacement[0];
    ins_bytes[3] = displacement[1];
    dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
  }
  break;

  case type::MEMORY_ADDRESSING_MODES::REG_IND:
  { // generates one instruction ==> 4B
    if (this->get_gp_reg_1() == type::GP_REG::NO_REG)
    {
      Assembler::get_instance().internal_error("Both GP registers must be set when reg ind mem addr mode is used with ld instruction.");
      return;
    }

    ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_2);
    ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_1()), static_cast<type::byte>(this->get_gp_reg_0()));
    displacement = converter::disp_to_byte_arr(0);
    ins_bytes[2] = displacement[0];
    ins_bytes[3] = displacement[1];
    dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
  }
  break;

  case type::MEMORY_ADDRESSING_MODES::REG_IND_WITH_DISP:
  { // generates one instruction ==> 4B
    if (this->get_gp_reg_1() == type::GP_REG::NO_REG)
    {
      Assembler::get_instance().internal_error("Both GP registers must be set when reg ind with disp mem addr mode is used with ld instruction.");
      return;
    }

    if (!param)
    {
      Assembler::get_instance().internal_error("Parameter is needed when reg ind with disp mem addr mode is used with ld instruction.");
      return;
    }

    int32_t reg_disp = 0;

    if (param->get_type() == type::PARAMETER_TYPE::SYMBOL)
    {
      Symbol *sym = (Symbol *)param;
      if (!sym->get_final_flag())
      {
        Assembler::get_instance().internal_error("Used symbol: \"" + sym->get_name() + std::string("\" doesn't have final value and can't be used as displacement within ld instruction."));
        return;
      }

      reg_disp = sym->get_value();
    }
    else if (param->get_type() == type::PARAMETER_TYPE::LITERAL)
    {
      reg_disp = ((Literal *)param)->get_num_value();
    }
    else
    {
      Assembler::get_instance().internal_error("Wrong parameter passed to ld instruction [reg ind with disp mem addr mode].");
      return;
    }

    // check if displacement (signed) value can be written in 12bit instruction field
    if (reg_disp < type::MAX_NEG_DISP || reg_disp > type::MAX_POS_DISP)
    {
      Assembler::get_instance().internal_error("Given displacement: " + std::to_string(reg_disp) + std::string(", can't fit in 12bits wide instruction field. Cannot generate ld instruction [reg ind with disp mem addr mode]."));
      return;
    }

    ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_2);
    ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_1()), static_cast<type::byte>(this->get_gp_reg_0()));
    displacement = converter::disp_to_byte_arr(reg_disp);
    ins_bytes[2] = displacement[0];
    ins_bytes[3] = displacement[1];
    dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
  }
  break;

  default:
  {
    Assembler::get_instance().internal_error("Wrong mem addr mode set for ld instruction.");
    return;
  }
  }
}

instruction::ST::ST()
{
  this->is_generating_data = true;
}

void instruction::ST::execute(Section *dest_section) const
{ // TODO: test ST
  // st %gpr, operand ==> operand <= gpr; [gpr = gp_reg_0]  OR  [gpr = gp_reg_0 | operand = gp_reg_1]

  if (this->get_gp_reg_0() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP register must be set to execute ld instruction.");
    return;
  }

  // XXX: is validation if gp reg is set to special gp registers (PC or SP) needed?

  if (this->get_mem_addr_mode() == type::MEMORY_ADDRESSING_MODES::NO_MODE)
  {
    Assembler::get_instance().internal_error("Memory addressing mode must be set to execute ld instruction.");
    return;
  }

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};
  std::array<type::byte, 2> displacement;
  Parameter *param = this->params.empty() ? nullptr : this->params.front();

  switch (this->get_mem_addr_mode())
  {
  case type::MEMORY_ADDRESSING_MODES::MEM_DIR:
  { // generates one instructions ==> 4B
    if (!param)
    {
      Assembler::get_instance().internal_error("Parameter is needed when mem dir addr mode is used with st instruction.");
      return;
    }

    if (param->get_type() == type::PARAMETER_TYPE::SYMBOL)
    {
      Symbol *sym = (Symbol *)param;
      if (!sym->get_defined_flag())
      {
        Assembler::get_instance().internal_error("Using undefined symbol: \"" + sym->get_name() + std::string("\". Cannot generate ld instruction [immed mem addr mode]."));
        return;
      }

      // insert new relocation record, because final value of symbol is needed
      /* TODO:
          if symbol is defined with .equ directive [can be checked with symbol's get_final_flag method],
          relocatable_flag (in LiteralPoolRecord) is false and there's no need for relocation
          or, if it can fit, place it in displacement of instruction
      */
      LiteralPoolRecord *literal_from_pool = new LiteralPoolRecord(sym->get_id(), true);
      dest_section->literal_pool_insert_new(literal_from_pool);

      // create relocation record for literal in pool, and add it to section's relocations collection
      int32_t sym_id = sym->get_global_flag() ? sym->get_id() : sym->get_section()->get_id();
      uint32_t addend = sym->get_global_flag() ? 0 : sym->get_value();
      RelocationRecord *rel_record = new RelocationRecord(literal_from_pool->get_address(), sym_id, type::RELOCATIONS::ABS_32U, addend);
      dest_section->add_new_relocation(rel_record);

      ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::ST_DATA_1);
      ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::PC), static_cast<type::byte>(type::GP_REG::R0));
      displacement = converter::disp_to_byte_arr(literal_from_pool->get_address() - dest_section->get_curr_loc_cnt() - this->get_size());
      converter::write_to_upper_byte_half(static_cast<type::byte>(this->get_gp_reg_0()), &displacement[0]);
      ins_bytes[2] = displacement[0];
      ins_bytes[3] = displacement[1];
      dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
    }
    else if (param->get_type() == type::PARAMETER_TYPE::LITERAL)
    {
      // literal passed as parameter
      uint32_t value = ((Literal *)param)->get_num_value();
      if (value <= type::MAX_UNSIGNED_DISP)
      {
        // literal value can fit in 12 bits
        ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::ST_DATA_0);
        ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::R0), static_cast<type::byte>(type::GP_REG::R0));
        displacement = converter::disp_to_byte_arr(value);
        converter::write_to_upper_byte_half(static_cast<type::byte>(this->get_gp_reg_0()), &displacement[0]);
        ins_bytes[2] = displacement[0];
        ins_bytes[3] = displacement[1];
        dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
      }
      else
      {
        // literal value can't fit in 12 bits, write in pool literal and, later, read from it
        LiteralPoolRecord *literal_from_pool = new LiteralPoolRecord(value, false);
        dest_section->literal_pool_insert_new(literal_from_pool);

        ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::ST_DATA_1);
        ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(type::GP_REG::PC), static_cast<type::byte>(type::GP_REG::R0));
        displacement = converter::disp_to_byte_arr(literal_from_pool->get_address() - dest_section->get_curr_loc_cnt() - this->get_size());
        converter::write_to_upper_byte_half(static_cast<type::byte>(this->get_gp_reg_0()), &displacement[0]);
        ins_bytes[2] = displacement[0];
        ins_bytes[3] = displacement[1];
        dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
      }
    }
    else
    {
      Assembler::get_instance().internal_error("Wrong parameter passed to ld instruction [immed mem addr mode].");
      return;
    }
  }
  break;

  case type::MEMORY_ADDRESSING_MODES::REG_DIR: // FIXME: this possibly shouldn't be allowed for ST instruction
  {                                            // generates one instruction ==> 4B
    if (this->get_gp_reg_1() == type::GP_REG::NO_REG)
    {
      Assembler::get_instance().internal_error("Both GP registers must be set when reg dir mem addr mode is used with st instruction.");
      return;
    }

    ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_1);
    ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_1()), static_cast<type::byte>(this->get_gp_reg_0()));
    displacement = converter::disp_to_byte_arr(0);
    ins_bytes[2] = displacement[0];
    ins_bytes[3] = displacement[1];
    dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
  }
  break;

  case type::MEMORY_ADDRESSING_MODES::REG_IND:
  { // generates one instruction ==> 4B
    if (this->get_gp_reg_1() == type::GP_REG::NO_REG)
    {
      Assembler::get_instance().internal_error("Both GP registers must be set when reg ind mem addr mode is used with st instruction.");
      return;
    }

    ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::ST_DATA_0);
    ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_1()), static_cast<type::byte>(type::GP_REG::R0));
    displacement = converter::disp_to_byte_arr(0);
    converter::write_to_upper_byte_half(static_cast<type::byte>(this->get_gp_reg_0()), &displacement[0]);
    ins_bytes[2] = displacement[0];
    ins_bytes[3] = displacement[1];
    dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
  }
  break;

  case type::MEMORY_ADDRESSING_MODES::REG_IND_WITH_DISP:
  { // generates one instruction ==> 4B
    if (this->get_gp_reg_1() == type::GP_REG::NO_REG)
    {
      Assembler::get_instance().internal_error("Both GP registers must be set when reg ind with disp mem addr mode is used with st instruction.");
      return;
    }

    if (!param)
    {
      Assembler::get_instance().internal_error("Parameter is needed when reg ind with disp mem addr mode is used with st instruction.");
      return;
    }

    int32_t reg_disp = 0;

    if (param->get_type() == type::PARAMETER_TYPE::SYMBOL)
    {
      Symbol *sym = (Symbol *)param;
      if (!sym->get_final_flag())
      {
        Assembler::get_instance().internal_error("Used symbol: \"" + sym->get_name() + std::string("\" doesn't have final value and can't be used as displacement within st instruction."));
        return;
      }

      reg_disp = sym->get_value();
    }
    else if (param->get_type() == type::PARAMETER_TYPE::LITERAL)
    {
      reg_disp = ((Literal *)param)->get_num_value();
    }
    else
    {
      Assembler::get_instance().internal_error("Wrong parameter passed to st instruction [reg ind with disp mem addr mode].");
      return;
    }

    // check if displacement (signed) value can be written in 12bit instruction field
    if (reg_disp < type::MAX_NEG_DISP || reg_disp > type::MAX_POS_DISP)
    {
      Assembler::get_instance().internal_error("Given displacement: " + std::to_string(reg_disp) + std::string(", can't fit in 12bits wide instruction field. Cannot generate st instruction [reg ind with disp mem addr mode]."));
      return;
    }

    ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::ST_DATA_0);
    ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_1()), static_cast<type::byte>(type::GP_REG::R0));
    displacement = converter::disp_to_byte_arr(reg_disp);
    converter::write_to_upper_byte_half(static_cast<type::byte>(this->get_gp_reg_0()), &displacement[0]);
    ins_bytes[2] = displacement[0];
    ins_bytes[3] = displacement[1];
    dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
  }
  break;

  default:
  {
    Assembler::get_instance().internal_error("Wrong mem addr mode set for st instruction.");
    return;
  }
  }
}

instruction::CSRRD::CSRRD()
{
  this->is_generating_data = true;
}

void instruction::CSRRD::execute(Section *dest_section) const
{
  // TODO: test CSRRD
  // csrrd %csr, %gpr ==> gpr <= csr; [csr = cs_reg_0 | gpr = gp_reg_0]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP register must be set to execute csrrd instruction.");
    return;
  }

  if (this->get_cs_reg_0() == type::CS_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("CS register must be set to execute csrrd instruction.");
    return;
  }

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};

  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_0);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_gp_reg_0()), static_cast<type::byte>(this->get_cs_reg_0()));

  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
}

instruction::CSRWR::CSRWR()
{
  this->is_generating_data = true;
}

void instruction::CSRWR::execute(Section *dest_section) const
{
  // TODO: test CSRWR
  // csrwr %gpr, %csr ==> csr <= gpr; [csr = cs_reg_0 | gpr = gp_reg_0]
  if (this->get_gp_reg_0() == type::GP_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("GP register must be set to execute csrwr instruction.");
    return;
  }

  if (this->get_cs_reg_0() == type::CS_REG::NO_REG)
  {
    Assembler::get_instance().internal_error("CS register must be set to execute csrwr instruction.");
    return;
  }

  std::array<type::byte, 4> ins_bytes = {0, 0, 0, 0};

  ins_bytes[0] = static_cast<type::byte>(type::CPU_INSTRUCTIONS::LD_DATA_4);
  ins_bytes[1] = converter::create_byte_of_two_halves(static_cast<type::byte>(this->get_cs_reg_0()), static_cast<type::byte>(this->get_gp_reg_0()));

  dest_section->write_byte_arr({ins_bytes.begin(), ins_bytes.end()});
}
