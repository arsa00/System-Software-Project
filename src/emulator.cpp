#include "../auxiliary/inc/converters.hpp"
#include "../inc/emulator.hpp"
#include <iostream>

Emulator &Emulator::get_instance()
{
  static Emulator emulator_instance;
  return emulator_instance;
}

Emulator::Emulator()
{
  this->gpr[0] = 0;
}

Emulator::~Emulator()
{
  // TODO
}

void Emulator::internal_error(std::string err_msg)
{
  this->internal_err = true;
  std::cerr << "[EMULATOR]: { INTERNAL ERROR: " << err_msg << " }" << std::endl;
}

void Emulator::read_memory()
{
  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t read_byte;
    if (this->memory.find(this->mar) == this->memory.end())
      read_byte = 0;
    else
      read_byte = static_cast<uint8_t>(this->memory[this->mar]);

    this->mdr = read_byte >> this->mdr;
    this->mar++;
  }
}

void Emulator::write_memory()
{
  auto write_bytes = converter::get_instruction_bytes(static_cast<type::instruction_size>(this->mdr));
  for (type::byte byte : write_bytes)
  {
    this->memory[this->mar++] = byte;
  }
}

void Emulator::push(uint32_t value)
{
  *this->sp -= 4;
  this->mar = *this->sp;
  this->mdr = value;
  this->write_memory();
}

uint32_t Emulator::pop()
{
  this->mar = *this->sp;
  this->sp += 4;
  this->read_memory();
  return this->mdr;
}

bool Emulator::is_timer_interrupt_enabled()
{
  return (*this->status & 0x1) == 0;
}

bool Emulator::is_terminal_interrupt_enabled()
{
  return (*this->status & 0x2) == 0;
}

bool Emulator::is_global_interrupt_enabled()
{
  return (*this->status & 0x4) == 0;
}

void Emulator::clear_interrupt_flag()
{
  this->interrupted = false;
}

void Emulator::set_interrupt_flag()
{
  this->interrupted = true;
}

void Emulator::fetch_instruction()
{
  this->mar = *this->pc;
  this->read_memory();
  this->ir = this->mdr;
  *this->pc += 4;
}

void Emulator::resolve_address()
{
  auto instruction_bytes = converter::get_instruction_bytes(static_cast<type::instruction_size>(this->ir));
  uint8_t oc_mod = static_cast<uint8_t>(instruction_bytes[0]);
  uint8_t regA = static_cast<uint8_t>(converter::get_upper_half_byte(instruction_bytes[1]));
  uint8_t regB = static_cast<uint8_t>(converter::get_lower_half_byte(instruction_bytes[1]));
  uint8_t regC = static_cast<uint8_t>(converter::get_upper_half_byte(instruction_bytes[2]));
  int16_t disp = converter::get_disp_from_instruction(static_cast<type::instruction_size>(this->ir));

  switch (static_cast<type::CPU_INSTRUCTIONS>(oc_mod))
  {
  case type::CPU_INSTRUCTIONS::CALL_1:
  {
    this->mar = this->gpr[regA] + this->gpr[regB] + disp;
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_4:
  case type::CPU_INSTRUCTIONS::JMP_5:
  case type::CPU_INSTRUCTIONS::JMP_6:
  case type::CPU_INSTRUCTIONS::JMP_7:
  {
    this->mar = this->gpr[regA] + disp;
    break;
  }

  case type::CPU_INSTRUCTIONS::ST_DATA_0:
  {
    this->mar = this->gpr[regA] + this->gpr[regB] + disp;
    break;
  }

  case type::CPU_INSTRUCTIONS::ST_DATA_1:
  {
    this->mar = this->gpr[regA] + this->gpr[regB] + disp;
    this->read_memory();
    this->mar = this->mdr;
    break;
  }

  case type::CPU_INSTRUCTIONS::ST_DATA_2:
  {
    this->gpr[regA] += disp;
    this->mar = this->gpr[regA];
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_2:
  case type::CPU_INSTRUCTIONS::LD_DATA_6:
  {
    this->mar = this->gpr[regB] + this->gpr[regC] + disp;
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_3:
  case type::CPU_INSTRUCTIONS::LD_DATA_7:
  {
    this->mar = this->gpr[regB];
    break;
  }
  }
}

void Emulator::execute_operation()
{
  auto instruction_bytes = converter::get_instruction_bytes(static_cast<type::instruction_size>(this->ir));
  uint8_t oc_mod = static_cast<uint8_t>(instruction_bytes[0]);
  uint8_t regA = static_cast<uint8_t>(converter::get_upper_half_byte(instruction_bytes[1]));
  uint8_t regB = static_cast<uint8_t>(converter::get_lower_half_byte(instruction_bytes[1]));
  uint8_t regC = static_cast<uint8_t>(converter::get_upper_half_byte(instruction_bytes[2]));
  int16_t disp = converter::get_disp_from_instruction(static_cast<type::instruction_size>(this->ir));

  switch (static_cast<type::CPU_INSTRUCTIONS>(oc_mod))
  {
  case type::CPU_INSTRUCTIONS::HALT:
  {
    this->is_running = false;
    break;
  }

  case type::CPU_INSTRUCTIONS::INT:
  {
    // TODO: see how to realize interrupts
    this->push(*this->status);
    this->push(*this->pc);
    *this->cause = 4;
    *this->status = *this->status & (~0x1); // what is this for? (enetering interrupt masks interrupts globally)
    *this->pc = *this->handler;
    this->set_interrupt_flag();
    break;
  }

  case type::CPU_INSTRUCTIONS::CALL_0:
  {
    this->push(*this->pc);
    *this->pc = this->gpr[regA] + this->gpr[regB] + disp;
    break;
  }

  case type::CPU_INSTRUCTIONS::CALL_1:
  {
    this->push(*this->pc);
    this->read_memory();
    *this->pc = this->mdr;
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_0:
  {
    *this->pc = this->gpr[regA] + disp;
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_1:
  {
    if (this->gpr[regB] == this->gpr[regC])
    {
      *this->pc = this->gpr[regA] + disp;
    }
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_2:
  {
    if (this->gpr[regB] != this->gpr[regC])
    {
      *this->pc = this->gpr[regA] + disp;
    }
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_3:
  {
    if ((int32_t)this->gpr[regB] > (int32_t)this->gpr[regC])
    {
      *this->pc = this->gpr[regA] + disp;
    }
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_4:
  {
    this->read_memory();
    *this->pc = this->mdr;
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_5:
  {
    if (this->gpr[regB] == this->gpr[regC])
    {
      this->read_memory();
      *this->pc = this->mdr;
    }
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_6:
  {
    if (this->gpr[regB] != this->gpr[regC])
    {
      this->read_memory();
      *this->pc = this->mdr;
    }
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_7:
  {
    if ((int32_t)this->gpr[regB] > (int32_t)this->gpr[regC])
    {
      this->read_memory();
      *this->pc = this->mdr;
    }
    break;
  }

  case type::CPU_INSTRUCTIONS::XCHG:
  {
    uint32_t temp = this->gpr[regB];
    this->gpr[regB] = this->gpr[regC];
    this->gpr[regC] = temp;
    break;
  }

  case type::CPU_INSTRUCTIONS::ARITH_OP_0:
  {
    this->gpr[regA] = this->gpr[regB] + this->gpr[regC];
    break;
  }

  case type::CPU_INSTRUCTIONS::ARITH_OP_1:
  {
    this->gpr[regA] = this->gpr[regB] - this->gpr[regC];
    break;
  }

  case type::CPU_INSTRUCTIONS::ARITH_OP_2:
  {
    this->gpr[regA] = this->gpr[regB] * this->gpr[regC];
    break;
  }

  case type::CPU_INSTRUCTIONS::ARITH_OP_3:
  {
    this->gpr[regA] = this->gpr[regB] / this->gpr[regC]; // TODO: check
    break;
  }

  case type::CPU_INSTRUCTIONS::LOGIC_OP_0:
  {
    this->gpr[regA] = ~this->gpr[regB];
    break;
  }

  case type::CPU_INSTRUCTIONS::LOGIC_OP_1:
  {
    this->gpr[regA] = this->gpr[regB] & this->gpr[regC];
    break;
  }

  case type::CPU_INSTRUCTIONS::LOGIC_OP_2:
  {
    this->gpr[regA] = this->gpr[regB] | this->gpr[regC];
    break;
  }

  case type::CPU_INSTRUCTIONS::LOGIC_OP_3:
  {
    this->gpr[regA] = this->gpr[regB] ^ this->gpr[regC];
    break;
  }

  case type::CPU_INSTRUCTIONS::SHIFT_OP_0:
  {
    this->gpr[regA] = this->gpr[regB] << this->gpr[regC];
    break;
  }

  case type::CPU_INSTRUCTIONS::SHIFT_OP_1:
  {
    this->gpr[regA] = this->gpr[regB] >> this->gpr[regC]; // TODO: check: (int32_t)this->gpr[regB] >> this->gpr[regC]
    break;
  }

  case type::CPU_INSTRUCTIONS::ST_DATA_0:
  case type::CPU_INSTRUCTIONS::ST_DATA_1:
  case type::CPU_INSTRUCTIONS::ST_DATA_2:
  {
    this->mdr = this->gpr[regC];
    this->write_memory();
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_0:
  {
    this->gpr[regA] = this->csr[regB];
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_1:
  {
    this->gpr[regA] = this->gpr[regB] + disp;
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_2:
  {
    this->read_memory();
    this->gpr[regA] = this->mdr;
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_3:
  {
    this->read_memory();
    this->gpr[regA] = this->mdr;
    this->gpr[regB] += disp;
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_4:
  {
    this->csr[regA] = this->gpr[regB];
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_5:
  {
    this->csr[regA] = this->csr[regB] | disp;
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_6:
  {
    this->read_memory();
    this->csr[regA] = this->mdr;
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_7:
  {
    this->read_memory();
    this->csr[regA] = this->mdr;
    this->gpr[regB] += disp;
    break;
  }

  default:
  {
    // TODO: see how to realize interrupts
    this->push(*this->status);
    this->push(*this->pc);
    *this->cause = 1;  // wrong operation code and modificator
    *this->status = 4; // mask global interrupts
    *this->pc = *this->handler;
    this->set_interrupt_flag();
    break;
  }
  }
}

void Emulator::handle_interrupts()
{
  // TODO
}

void Emulator::run()
{
  this->is_running = true;
  *this->pc = Emulator::START_MEM_ADDR;
  *this->sp = Emulator::MEM_MAPPED_REGS_START; // TODO: check if start addr for stack is correct

  while (this->is_running)
  {
    this->fetch_instruction();
    this->resolve_address();
    this->execute_operation();
    this->handle_interrupts();
  }
}