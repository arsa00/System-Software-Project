#ifndef COMMAND_BUILDER_HPP
#define COMMAND_BUILDER_HPP

#include "instruction.hpp"
#include "directive.hpp"

class CommandBuilder
{
private:
  std::list<Parameter*> params;
  type::MEMORY_ADDRESSING_MODES mem_addr_mode;

  CommandBuilder() { }
  CommandBuilder(const CommandBuilder &) = delete;
  CommandBuilder& operator= (const CommandBuilder &) = delete;
public:
  static CommandBuilder& get_instance();
  Instruction* build_instruction(type::INSTRUCTION_TYPE);
  Directive* build_directive(type::DIRECTIVE_TYPE);

  void set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES);
  void set_params(const std::list<Parameter*> &params);
  std::list<Parameter*> get_params() const;
  void delete_params();
  void clear_params();
  void enque_param(Parameter *param);
  Parameter* deque_param();
};

#endif