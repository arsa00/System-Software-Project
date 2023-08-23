#ifndef SECTION_HPP
#define SECTION_HPP

#include "instruction.hpp"
#include "directive.hpp"
#include "types.hpp"

#include <string>
#include <list>
#include <vector>

class Section : public Parameter
{
private:
  std::string name;
  std::list<Command *> commands;
  unsigned int length = 0;
  unsigned int location_counter = 0;

  std::vector<type::byte> output_file;

public:
  Section(std::string name);
  Section(char *name);
  ~Section();

  // no need for copying or moving operations
  Section(const Section &) = delete;
  Section &operator=(const Section &) = delete;

  unsigned int get_curr_loc_cnt() const;
  unsigned int get_length() const;

  void set_name(std::string);
  void set_name(char *);
  std::string get_name() const;

  void add_command(Command *);
  std::list<Command *> get_all_commands() const;
  void execute_all_commands();

  void write_byte_arr(std::vector<type::byte> arr);
  void write_byte(type::byte single_byte);
};

#endif