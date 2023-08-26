#include "../inc/section.hpp"
#include <iostream>
#include <iomanip>
#include <bitset>

Section::Section(std::string name) : Parameter(type::PARAMETER_TYPE::SECTION)
{
  this->name = name;
}

Section::Section(char *name) : Parameter(type::PARAMETER_TYPE::SECTION)
{
  this->name.assign(name);
}

Section::~Section()
{
  while (!this->commands.empty())
  {
    auto cmd = this->commands.front();
    this->commands.pop_front();
    delete cmd;
  }
}

void Section::set_name(std::string name)
{
  this->name = name;
}

void Section::set_name(char *name)
{
  this->name.assign(name);
}

std::string Section::get_name() const
{
  return this->name;
}

uint32_t Section::get_curr_loc_cnt() const
{
  return this->location_counter;
}

uint32_t Section::get_length() const
{
  return this->length;
}

void Section::add_command(Command *cmd)
{
  this->commands.push_back(cmd);
  this->location_counter += cmd->get_size();
  this->length = this->location_counter;
}

std::list<Command *> Section::get_all_commands() const
{
  return this->commands;
}

void Section::execute_all_commands()
{
  this->location_counter = 0;
  // TODO: finish implementation
}

std::vector<type::byte> Section::get_output_file() const
{
  return this->output_file;
}

void Section::print_output_file(type::byte line_width, type::byte mode) const
{ // TODO: maybe add memory address printing?
  if (!line_width)
    line_width = 1;

  std::cout << " | ";
  type::byte new_line_cnt = 0;
  for (type::byte single_byte : this->output_file)
  {
    if (!mode)
      std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)single_byte << " | ";
    else if (mode == 1)
      std::cout << "0b" << std::setfill('0') << std::setw(8) << std::bitset<8>(single_byte) << " | ";

    if (++new_line_cnt % line_width == 0)
    {
      std::cout << std::endl;

      if (new_line_cnt < this->output_file.size())
        std::cout << " | ";
    }
  }

  if (new_line_cnt < line_width)
    std::cout << std::endl;
}

void Section::write_byte_arr(std::vector<type::byte> arr)
{
  for (type::byte single_byte : arr)
    this->write_byte(single_byte);
}

void Section::write_byte(type::byte single_byte)
{
  this->output_file.push_back(single_byte);
  this->location_counter++;
  // TODO: ^^^ Double check if this is the way to go. Maybe add full instruction size (to loc. cnt.) before execution and just check after (compare value of loc. cnt. and size of output_file)?
}

// adds literal to pool and set it's address
void Section::literal_pool_insert_new(LiteralPoolRecord *record)
{
  LiteralPoolKey key(record->get_value(), record->get_relocation_flag());

  // check if record already exists
  if (this->literal_pool.find(key) != this->literal_pool.end())
    return;

  record->set_address(this->get_length() + this->literal_pool.size());

  this->literal_pool[key] = record;
}

LiteralPoolRecord *Section::literal_pool_get(uint32_t liter_value, bool relocation_flag)
{
  LiteralPoolKey key(liter_value, relocation_flag);

  // check if record exists
  if (this->literal_pool.find(key) == this->literal_pool.end())
    return nullptr;

  return this->literal_pool[key];
}