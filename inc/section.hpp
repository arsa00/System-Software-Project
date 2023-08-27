#ifndef SECTION_HPP
#define SECTION_HPP

#include "instruction.hpp"
#include "directive.hpp"
#include "literal_pool_record.hpp"
#include "relocation_record.hpp"
#include "types.hpp"

#include <string>
#include <list>
#include <vector>
#include <unordered_map>

class Section : public Parameter
{
private:
  std::string name;
  std::list<Command *> commands;
  uint32_t length = 0;
  uint32_t location_counter = 0;

  std::unordered_map<LiteralPoolKey, LiteralPoolRecord *, LiteralPoolKeyHasher> literal_pool;
  std::list<RelocationRecord *> relocations;

  std::vector<type::byte> output_file;

public:
  Section(std::string name);
  Section(char *name);
  ~Section();

  // no need for copying or moving operations
  Section(const Section &) = delete;
  Section &operator=(const Section &) = delete;

  uint32_t get_curr_loc_cnt() const;
  uint32_t get_length() const;

  void set_name(std::string);
  void set_name(char *);
  std::string get_name() const;

  void add_command(Command *);
  std::list<Command *> get_all_commands() const;
  void create_output_file();

  std::vector<type::byte> get_output_file() const;
  void print_output_file(type::byte line_width = 4, type::byte mode = 0) const;

  void write_byte_arr(std::vector<type::byte> arr);
  void write_byte(type::byte single_byte);

  void literal_pool_insert_new(LiteralPoolRecord *record);
  LiteralPoolRecord *literal_pool_get(uint32_t liter_value, bool relocation_flag = false);

  void add_new_relocation(RelocationRecord *rela_record);
  std::list<RelocationRecord *> get_all_relocations() const;
};

#endif