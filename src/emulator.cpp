#include "../auxiliary/inc/converters.hpp"
#include "../inc/emulator.hpp"
#include <iostream>
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include <regex>
#include <sys/ioctl.h>

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

bool Emulator::load_memory_hex_from_file(std::string file_name)
{
  std::ifstream file_handle(file_name);

  if (!file_handle)
  {
    this->internal_error("Cannot read from hex file: " + file_name);
    return false;
  }

  std::regex e("[0-9a-fA-F]*:( +[0-9a-fA-F]{2})+$");
  std::string line;
  while (std::getline(file_handle, line))
  {
    // check if fetched line is valid
    if (!std::regex_match(line, e))
    {
      this->internal_error("Hex file is invalid, invalid_line: " + line);
      return false;
    }

    // parse the line into address and (value) content parts
    std::string delimiter1 = ":";
    std::string start_addr = line.substr(0, line.find(delimiter1));
    std::string content = line.substr(line.find(delimiter1) + delimiter1.length());
    // std::cout << "start_addr: " << start_addr << std::endl;
    // std::cout << "content: " << content << std::endl;

    std::string delimiter2 = " ";
    std::size_t pos;
    uint32_t curr_addr = (uint32_t)std::stoul(start_addr, 0, 16);
    while ((pos = content.find(delimiter2)) != std::string::npos)
    {
      // fetch next value in row
      std::string token = content.substr(0, pos);
      content.erase(0, pos + delimiter2.length());

      if (token.empty())
        continue;

      // check if the addr that is written into is not forbidden (e.g. MEM_MAPPED_REGS_ADDR)
      if (curr_addr >= Emulator::MEM_MAPPED_REGS_START && curr_addr <= Emulator::MEM_MAPPED_REGS_END)
      {
        this->internal_error("Memory access violation, addr: " + converter::uint32_to_hex_string(curr_addr));
        return false;
      }

      // insert data into memory
      // std::cout << token << std::endl;
      this->memory[curr_addr] = static_cast<type::byte>(std::stoul(token, 0, 16));
      // std::cout << "written at " << std::to_string(curr_addr) << std::endl;
      curr_addr++;
    }

    // insert data into memory
    // std::cout << content << std::endl;
    this->memory[curr_addr] = std::stoul(content, 0, 16);
  }

  file_handle.close();
  return true;
}

void Emulator::read_memory()
{
  // std::cout << "read from " << std::to_string(this->mar) << std::endl;
  this->mdr = 0;
  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t read_byte;
    if (this->memory.find(this->mar) == this->memory.end())
      read_byte = 0;
    else
      read_byte = static_cast<uint8_t>(this->memory[this->mar]);

    this->mdr = this->mdr | ((uint32_t)read_byte << i * 8);
    this->mar++;
  }
  // std::cout << "read from " << std::to_string(this->mar) << std::endl;
}

void Emulator::write_memory()
{
  bool is_write_to_term_out = (this->mar == Emulator::TERM_OUT_REG_ADDR ? true : false);

  // write to memory
  auto write_bytes = converter::get_instruction_bytes(static_cast<type::instruction_size>(this->mdr));
  for (type::byte byte : write_bytes)
  {
    this->memory[this->mar++] = byte;
  }

  // notify terminal if write was to term_out reg
  if (is_write_to_term_out)
  {
    this->term_out_has_value = true;
    std::cout << (char)this->mdr << std::flush;
    // this->notify_terminal();
    // std::cout << "term_out wr: " << (char)this->mdr << std::endl;
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

bool terminal_ready = false;
void Emulator::notify_terminal()
{
  // std::cout << "notify_terminal..start" << std::endl;
  // lock release
  std::lock_guard<std::mutex> lock(this->terminal_mutex);

  terminal_ready = true;

  // notify consumer
  this->terminal_cv.notify_one();
  // std::cout << "notify_terminal..end" << std::endl;
}

void Emulator::write_term_in_reg(uint32_t val)
{
  this->memory_mutex.lock();
  this->mar = Emulator::TERM_IN_REG_ADDR;
  this->mdr = val;
  this->write_memory();
  this->memory_mutex.unlock();
}

uint32_t Emulator::read_term_in_reg()
{
  this->memory_mutex.lock();
  this->mar = Emulator::TERM_IN_REG_ADDR;
  this->read_memory();
  this->memory_mutex.unlock();
  return this->mdr;
}

void Emulator::write_term_out_reg(uint32_t val)
{
  this->memory_mutex.lock();
  this->mar = Emulator::TERM_OUT_REG_ADDR;
  this->mdr = val;
  this->write_memory();
  this->memory_mutex.unlock();
}

uint32_t Emulator::read_term_out_reg()
{
  // std::cout << std::endl << "TERM WAIT MEM LOCK" << std::endl;
  this->memory_mutex.lock();
  // std::cout << std::endl << "TERM GOT MEM LOCK" << std::endl;
  this->mar = Emulator::TERM_OUT_REG_ADDR;
  this->read_memory();
  this->memory_mutex.unlock();
  return this->mdr;
}

bool Emulator::is_timer_interrupt_enabled()
{
  return (*this->status & Emulator::TIMER_INTERRUPT_FLAG) == 0;
}

bool Emulator::is_terminal_interrupt_enabled()
{
  return (*this->status & Emulator::TERMINAL_INTERRUPT_FLAG) == 0;
}

bool Emulator::is_global_interrupt_enabled()
{
  return (*this->status & Emulator::GLOBAL_INTERRUPT_FLAG) == 0;
}

void Emulator::fetch_instruction()
{
  if (this->verbose_print)
  {
    std::cout << "> " << converter::uint32_to_hex_string(*this->pc) << std::endl;
  }

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
    // std::cout << "CALL1_MEM_ADDR: " << converter::uint32_to_hex_string(this->mar) << std::endl;
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
    // std::cout << "ST_DATA_0_MEM_ADDR: " << converter::uint32_to_hex_string(this->mar) << std::endl;
    break;
  }

  case type::CPU_INSTRUCTIONS::ST_DATA_1:
  {
    this->mar = this->gpr[regA] + this->gpr[regB] + disp;
    // std::cout << "ST_DATA_1_MEM_ADDR(0): " << converter::uint32_to_hex_string(this->mar) << std::endl;
    this->read_memory();
    this->mar = this->mdr;
    // std::cout << "ST_DATA_1_MEM_ADDR(1): " << converter::uint32_to_hex_string(this->mar) << std::endl;
    break;
  }

  case type::CPU_INSTRUCTIONS::ST_DATA_2:
  {
    this->gpr[regA] += disp;
    this->mar = this->gpr[regA];
    // std::cout << "ST_DATA_2_MEM_ADDR: " << converter::uint32_to_hex_string(this->mar) << std::endl;
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

  // std::cout << "executing: " << std::hex << std::to_string(this->ir) << std::endl;

  switch (static_cast<type::CPU_INSTRUCTIONS>(oc_mod))
  {
  case type::CPU_INSTRUCTIONS::HALT:
  {
    // std::cout << "executing: HALT" << std::endl;
    this->is_running = false;
    break;
  }

  case type::CPU_INSTRUCTIONS::INT:
  {
    // TODO: check this
    // std::cout << "executing: INT" << std::endl;
    this->push(*this->status);
    this->push(*this->pc);
    *this->cause = 4;
    *this->status = *this->status & (~0x1);
    *this->pc = *this->handler;
    break;
  }

  case type::CPU_INSTRUCTIONS::CALL_0:
  {
    // std::cout << "executing: CALL_0" << std::endl;
    this->push(*this->pc);
    *this->pc = this->gpr[regA] + this->gpr[regB] + disp;
    // std::cout << "executing: CALL_0; new_pc = " << converter::uint32_to_hex_string(*this->pc) << std::endl;
    break;
  }

  case type::CPU_INSTRUCTIONS::CALL_1:
  {
    this->read_memory();
    uint32_t new_pc = this->mdr;
    this->push(*this->pc);
    *this->pc = new_pc;
    // std::cout << "executing: CALL_1; new_pc = " << converter::uint32_to_hex_string(*this->pc) << std::endl;
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_0:
  {
    // std::cout << "executing: JMP_0" << std::endl;
    *this->pc = this->gpr[regA] + disp;
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_1:
  {
    // std::cout << "executing: JMP_1" << std::endl;
    if (this->gpr[regB] == this->gpr[regC])
    {
      *this->pc = this->gpr[regA] + disp;
    }
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_2:
  {
    // std::cout << "executing: JMP_2" << std::endl;
    if (this->gpr[regB] != this->gpr[regC])
    {
      *this->pc = this->gpr[regA] + disp;
    }
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_3:
  {
    // std::cout << "executing: JMP_3" << std::endl;
    if ((int32_t)this->gpr[regB] > (int32_t)this->gpr[regC])
    {
      *this->pc = this->gpr[regA] + disp;
    }
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_4:
  {
    // std::cout << "executing: JMP_4" << std::endl;
    this->read_memory();
    *this->pc = this->mdr;
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_5:
  {
    // std::cout << "executing: JMP_5" << std::endl;
    if (this->gpr[regB] == this->gpr[regC])
    {
      this->read_memory();
      *this->pc = this->mdr;
    }
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_6:
  {
    // std::cout << "executing: JMP_6" << std::endl;
    if (this->gpr[regB] != this->gpr[regC])
    {
      this->read_memory();
      *this->pc = this->mdr;
    }
    break;
  }

  case type::CPU_INSTRUCTIONS::JMP_7:
  {
    // std::cout << "executing: JMP_7" << std::endl;
    if ((int32_t)this->gpr[regB] > (int32_t)this->gpr[regC])
    {
      this->read_memory();
      *this->pc = this->mdr;
    }
    break;
  }

  case type::CPU_INSTRUCTIONS::XCHG:
  {
    // std::cout << "executing: XCHG" << std::endl;
    uint32_t temp = this->gpr[regB];
    this->gpr[regB] = this->gpr[regC];
    this->gpr[regC] = temp;
    break;
  }

  case type::CPU_INSTRUCTIONS::ARITH_OP_0:
  {
    // std::cout << "executing: ARITH_OP_0" << std::endl;
    this->gpr[regA] = this->gpr[regB] + this->gpr[regC];
    break;
  }

  case type::CPU_INSTRUCTIONS::ARITH_OP_1:
  {
    // std::cout << "executing: ARITH_OP_1" << std::endl;
    this->gpr[regA] = this->gpr[regB] - this->gpr[regC];
    break;
  }

  case type::CPU_INSTRUCTIONS::ARITH_OP_2:
  {
    // std::cout << "executing: ARITH_OP_2" << std::endl;
    this->gpr[regA] = this->gpr[regB] * this->gpr[regC];
    break;
  }

  case type::CPU_INSTRUCTIONS::ARITH_OP_3:
  {
    // std::cout << "executing: ARITH_OP_3" << std::endl;
    this->gpr[regA] = (int32_t)this->gpr[regB] / (int32_t)this->gpr[regC]; // TODO: check
    break;
  }

  case type::CPU_INSTRUCTIONS::LOGIC_OP_0:
  {
    // std::cout << "executing: LOGIC_OP_0" << std::endl;
    this->gpr[regA] = ~this->gpr[regB];
    break;
  }

  case type::CPU_INSTRUCTIONS::LOGIC_OP_1:
  {
    // std::cout << "executing: LOGIC_OP_1" << std::endl;
    this->gpr[regA] = this->gpr[regB] & this->gpr[regC];
    break;
  }

  case type::CPU_INSTRUCTIONS::LOGIC_OP_2:
  {
    // std::cout << "executing: LOGIC_OP_2" << std::endl;
    this->gpr[regA] = this->gpr[regB] | this->gpr[regC];
    break;
  }

  case type::CPU_INSTRUCTIONS::LOGIC_OP_3:
  {
    // std::cout << "executing: LOGIC_OP_3" << std::endl;
    this->gpr[regA] = this->gpr[regB] ^ this->gpr[regC];
    break;
  }

  case type::CPU_INSTRUCTIONS::SHIFT_OP_0:
  {
    // std::cout << "executing: SHIFT_OP_0" << std::endl;
    this->gpr[regA] = this->gpr[regB] << this->gpr[regC];
    break;
  }

  case type::CPU_INSTRUCTIONS::SHIFT_OP_1:
  {
    // std::cout << "executing: SHIFT_OP_1" << std::endl;
    this->gpr[regA] = (int32_t)this->gpr[regB] >> this->gpr[regC]; // TODO: check: this->gpr[regB] >> this->gpr[regC]
    break;
  }

  case type::CPU_INSTRUCTIONS::ST_DATA_0:
  case type::CPU_INSTRUCTIONS::ST_DATA_1:
  case type::CPU_INSTRUCTIONS::ST_DATA_2:
  {
    // std::cout << "executing: ST_DATA_0/1/2" << std::endl;
    this->mdr = this->gpr[regC];
    this->write_memory();
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_0:
  {
    // std::cout << "executing: LD_DATA_0" << std::endl;
    this->gpr[regA] = this->csr[regB];
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_1:
  {
    // std::cout << "executing: LD_DATA_1" << std::endl;
    this->gpr[regA] = this->gpr[regB] + disp;
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_2:
  {
    // std::cout << "executing: LD_DATA_2" << std::endl;
    this->read_memory();
    this->gpr[regA] = this->mdr;
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_3:
  {
    // std::cout << "executing: LD_DATA_3" << std::endl;
    this->read_memory();
    this->gpr[regA] = this->mdr;
    this->gpr[regB] += disp;
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_4:
  {
    // std::cout << "executing: LD_DATA_4" << std::endl;
    this->csr[regA] = this->gpr[regB];
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_5:
  {
    // std::cout << "executing: LD_DATA_5" << std::endl;
    this->csr[regA] = this->csr[regB] | disp;
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_6:
  {
    // std::cout << "executing: LD_DATA_6" << std::endl;
    this->read_memory();
    this->csr[regA] = this->mdr;
    break;
  }

  case type::CPU_INSTRUCTIONS::LD_DATA_7:
  {
    // std::cout << "executing: LD_DATA_7" << std::endl;
    this->read_memory();
    this->csr[regA] = this->mdr;
    this->gpr[regB] += disp;
    break;
  }

  default:
  {
    // std::cout << "executing: INVALID" << std::endl;
    this->interrupt_invalid_op = true; // TODO: check if this interrupt should be set elsewhere
    break;
  }
  }
}

void Emulator::handle_interrupts()
{
  // TODO: check
  bool interrupt_accepted = false;
  if (this->interrupt_invalid_op)
  {
    // std::cout << "INTERRUPT: interrupt_invalid_op" << std::endl;
    *this->cause = 1; // wrong operation code, modificator, etc.
    interrupt_accepted = true;
    this->interrupt_invalid_op = false;
  }
  else if (this->interrupt_timer && this->is_global_interrupt_enabled() && this->is_timer_interrupt_enabled())
  {
    // std::cout << "INTERRUPT: interrupt_timer" << std::endl;
    *this->cause = 2; // timer interrupt
    interrupt_accepted = true;
    this->interrupt_timer = false;
  }
  else if (this->interrupt_terminal && this->is_global_interrupt_enabled() && this->is_terminal_interrupt_enabled())
  {
    // uint32_t temp = this->mar;
    // this->mar = Emulator::TERM_IN_REG_ADDR;
    // this->read_memory();
    // this->mar = temp;
    // std::cout << "INTERRUPT: interrupt_terminal: " << (char)this->mdr << std::endl;
    *this->cause = 3; // terminal interrupt
    interrupt_accepted = true;
    this->interrupt_terminal = false;
  }

  if (interrupt_accepted)
  {
    this->push(*this->status);
    this->push(*this->pc);
    *this->status |= Emulator::GLOBAL_INTERRUPT_FLAG; // mask global interrupts
    *this->pc = *this->handler;
  }
}

void Emulator::output_terminal_func()
{
  while (this->is_running)
  {
    // wake up once there is a data to write or emulator finished running

    // locking
    std::unique_lock<std::mutex> lock(this->terminal_mutex);

    // waiting
    // std::cout << std::endl << "term sleep" << std::endl;
    this->terminal_cv.wait(lock, [] { return terminal_ready; });

    // reset terminal ready flag
    terminal_ready = false;
    // std::cout << std::endl << "term woke" << std::endl;

    if (this->term_out_has_value)
    {
      char ch = (char)this->read_term_out_reg();
      // putc(ch, stdout);
      // putchar(ch);
      std::cout << ch << std::flush;
      this->term_out_has_value = false;
      // std::cout << "TERMINAL_OUW wrote: " << ch << std::endl;
    }
  }
}

void Emulator::input_terminal_func()
{
  int key_pressed;

  while (this->is_running)
  {
    ioctl(0, FIONREAD, &key_pressed);
    if (key_pressed > 0)
    {
      char ch = getchar();
      this->write_term_in_reg((uint32_t)ch);
      this->interrupt_terminal = true;
    }
    else
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void Emulator::run()
{
  this->is_running = true;
  *this->pc = Emulator::START_MEM_ADDR;
  // *this->sp = Emulator::MEM_MAPPED_REGS_START; // TODO: check if start addr for stack is correct

  // Save the original terminal settings
  termios original;
  tcgetattr(STDIN_FILENO, &original);

  // Create a new terminal settings
  termios newt = original;

  // Disable canonical mode and echo
  newt.c_lflag &= ~(ICANON | ECHO);

  // Apply the new settings
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  // Run terminal threads
  // std::thread terminal_output_thread(&Emulator::output_terminal_func, this);
  std::thread terminal_input_thread(&Emulator::input_terminal_func, this);

  // Execute emulator
  while (this->is_running)
  {
    // lock the memory
    this->memory_mutex.lock();

    this->fetch_instruction();
    this->resolve_address();
    this->execute_operation();
    this->handle_interrupts();

    // unlock the memory
    this->memory_mutex.unlock();

    // if (this->term_out_has_value && this->is_running)
    // {
    //   this->notify_terminal();
    //   // while(terminal_ready);
    // }
  }

  // Notify terminal that the emulator stopped running
  // this->notify_terminal();

  // Wait for terminal threads to finish
  // terminal_output_thread.join();
  terminal_input_thread.join();

  // Restore the original settings
  tcsetattr(STDIN_FILENO, TCSANOW, &original);
  this->print_state();
}

void Emulator::print_state()
{
  std::cout << std::endl << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Emulated processor executed halt instruction" << std::endl;
  std::cout << "Emulated processor state:" << std::endl;

  for (uint8_t i = 0; i < 16; i++)
  {
    std::cout << "r" + std::to_string(i) << "=0x" << std::setfill('0') << std::setw(8) << std::hex << this->gpr[i];
    if (i % 4 == 3)
      std::cout << std::endl;
    else
      std::cout << " ";
  }
}