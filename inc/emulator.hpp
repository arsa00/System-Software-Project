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
  const uint32_t MEM_MAPPED_REGS_START = 0xFFFFFF00; // 256 bytes long
  const uint32_t MEM_MAPPED_REGS_END = 0xFFFFFFFF;

  std::unordered_map<uint32_t, type::byte> memory;

  // general purpose registers
  uint32_t gpr[16];
  uint32_t *pc = &gpr[15];
  uint32_t *sp = &gpr[14]; // full stack - grows down

  // control-status registers
  uint32_t csr[3];
  uint32_t *status = &csr[0];
  uint32_t *handler = &csr[1];
  uint32_t *cause = &csr[2];

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
  void clear_interrupt_flag();

  void read_memory();
  void write_memory();
  void push(uint32_t value);
  uint32_t pop();

  void fetch_instruction();
  void resolve_address();
  void execute_operation();
  void handle_interrupts();

public:
  static Emulator &get_instance();

  void internal_error(std::string err_msg);
  void run();
  bool load_memory_hex_from_file(std::string file_name);
};

#endif