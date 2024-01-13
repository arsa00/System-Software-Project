#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include "../auxiliary/inc/object_file.hpp"
#include <string>
#include <unordered_map>
#include <list>
#include <vector>
#include <functional>
#include <iomanip>

class Emulator
{
private:
  const uint32_t START_MEM_ADDR = 0x40000000;
  const uint32_t MEM_MAPPED_REGS_START = 0xFFFFFF00;
  const uint32_t MEM_MAPPED_REGS_SIZE = 256;

  std::unordered_map<uint32_t, type::byte> memory;

  uint32_t gpr[16];
  uint32_t *pc = &gpr[15];
  uint32_t *sp = &gpr[14]; // full stack - grows down

  uint32_t handler;
  uint32_t status;
  uint32_t cause;

  // CPU registers
  uint32_t ir;  // instruction register
  uint32_t mar; // memory address register
  uint32_t mdr; // memory data register

  // interrupt flag
  bool interrupted;

  bool is_running = false;
  bool internal_err = false;

  Emulator();
  Emulator(const Emulator &) = delete;
  Emulator &operator=(const Emulator &) = delete;
  ~Emulator();

  bool is_terminal_interrupt_enabled();
  bool is_timer_interrupt_enabled();
  bool is_global_interrupt_enabled();

  void set_interrupt_flag();
  void get_interrupt_flag();

  void read_memory();

  void fetch_instruction();
  void resolve_address();
  void load_operands();
  void execute_operation();
  void handle_interrupts();

public:
  static Emulator &get_instance();

  void internal_error(std::string err_msg);
  void run();
  bool load_memory_hex_from_file(std::string file_name);
};

#endif