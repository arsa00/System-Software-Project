#include "../auxiliary/inc/converters.hpp"
#include "../inc/emulator.hpp"

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
  }
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

  switch (oc_mod)
  {
  case type::CPU_INSTRUCTIONS::CALL_1:
    this->mar = this->gpr[regA] + this->gpr[regB] + disp;
    break;

  case type::CPU_INSTRUCTIONS::JMP_4:
  case type::CPU_INSTRUCTIONS::JMP_5:
  case type::CPU_INSTRUCTIONS::JMP_6:
  case type::CPU_INSTRUCTIONS::JMP_7:
    this->mar = this->gpr[regA] + disp;
    break;

  case type::CPU_INSTRUCTIONS::ST_DATA_0:
    this->mar = this->gpr[regA] + this->gpr[regB] + disp;
    break;

  case type::CPU_INSTRUCTIONS::ST_DATA_1:
    this->mar = this->gpr[regA] + this->gpr[regB] + disp;
    this->read_memory();
    this->mar = this->mdr;
    break;

  case type::CPU_INSTRUCTIONS::ST_DATA_2:
    this->mar = this->gpr[regA];
    break;

  case type::CPU_INSTRUCTIONS::LD_DATA_2:
  case type::CPU_INSTRUCTIONS::LD_DATA_6:
    this->mar = this->gpr[regB] + this->gpr[regC] + disp;
    break;

  case type::CPU_INSTRUCTIONS::LD_DATA_3:
  case type::CPU_INSTRUCTIONS::LD_DATA_7:
    this->mar = this->gpr[regB];
    break;
  }
}

void Emulator::load_operands()
{
  // TODO
}

void Emulator::execute_operation()
{
  auto instruction_bytes = converter::get_instruction_bytes(static_cast<type::instruction_size>(this->ir));
  uint8_t oc_mod = static_cast<uint8_t>(instruction_bytes[0]);
  uint8_t regA = static_cast<uint8_t>(converter::get_upper_half_byte(instruction_bytes[1]));
  uint8_t regB = static_cast<uint8_t>(converter::get_lower_half_byte(instruction_bytes[1]));
  uint8_t regC = static_cast<uint8_t>(converter::get_upper_half_byte(instruction_bytes[2]));
  int16_t disp = converter::get_disp_from_instruction(static_cast<type::instruction_size>(this->ir));

  switch (oc_mod)
  {
  case type::CPU_INSTRUCTIONS::HALT:
    this->is_running = false;
    break;

  case type::CPU_INSTRUCTIONS::INT:
    // this->push(this->status);
    // this->push(*this->pc);
    this->cause = 4;
    this->status = this->status & (~0x1);
    *this->pc = this->handle;
    break;

  case type::CPU_INSTRUCTIONS::CALL_0:
    // this->push(*this->pc);
    *this->pc = this->gpr[regA] + this->gpr[regB] + disp;
    break;

  case type::CPU_INSTRUCTIONS::CALL_1:
    // this->push(*this->pc);
    this->read_memory();
    *this->pc = this->mdr;
    break;

  case type::CPU_INSTRUCTIONS::JMP_0:
    *this->pc = this->gpr[regA] + disp;
    break;

  case type::CPU_INSTRUCTIONS::JMP_1:
    if (this->gpr[regB] == this->gpr[regC])
    {
      *this->pc = this->gpr[regA] + disp;
    }
    break;

  default:
    break;
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

  while (this->is_running)
  {
    this->fetch_instruction();
    this->resolve_address();
    this->load_operands();
    this->execute_operation();
    this->handle_interrupts();
  }
}