#include "../inc/section.hpp"
#include "../auxiliary/inc/converters.hpp"
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
  // free memory fo all commands
  while (!this->commands.empty())
  {
    auto cmd = this->commands.front();
    this->commands.pop_front();
    delete cmd;
  }

  // free memory for all literal pool records
  while (!this->literal_pool.empty())
  {
    auto iter = this->literal_pool.begin();
    this->literal_pool.erase(iter);
    delete iter->second;
  }

  // free memory for all relocation records
  while (!this->relocations.empty())
  {
    auto iter = this->relocations.begin();
    this->relocations.erase(iter);
    delete *iter;
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

void Section::create_output_file()
{ // TODO: test implementation
  this->location_counter = 0;

  // execute all commands
  for (Command *cmd : this->commands)
  {
    cmd->execute(this);
  }

  // after executing all commands, exapnd output_file and insert literal pool records in output file
  this->output_file.resize(this->output_file.size() + this->literal_pool.size() * 4, 0);
  for (auto &iter : this->literal_pool)
  {
    LiteralPoolRecord *literal_pool_record = iter.second;
    type::addr_size addr = literal_pool_record->get_address();
    std::array<type::byte, 4> value_bytes = converter::get_instruction_bytes(literal_pool_record->get_value());
    this->output_file[addr + 0] = value_bytes[0];
    this->output_file[addr + 1] = value_bytes[1];
    this->output_file[addr + 2] = value_bytes[2];
    this->output_file[addr + 3] = value_bytes[3];
  }

  // XXX: check if it's needed to create section's relocation table???
}

std::vector<type::byte> Section::get_output_file() const
{
  return this->output_file;
}

void Section::print_output_file(type::byte line_width, type::byte mode) const
{
  if (!line_width)
    line_width = 1;

  std::cout << "SIZE: " << this->get_length() << " Lit_pool size: " << this->literal_pool.size() * 4 << std::endl;

  std::cout << "0x00: | ";
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
        std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)new_line_cnt << ": | ";
    }
  }

  std::cout << std::endl
            << "Literal pool: " << std::endl;
  type::byte new_line_cnt2 = 0;
  for (auto &iter : this->literal_pool)
  { // XXX: addresses don't go with asc order, 'cause it is unordered_map!!!
    LiteralPoolRecord *literal_from_pool = iter.second;

    if (new_line_cnt2 >= 0)
      std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)(new_line_cnt + new_line_cnt2)
                << "/" << literal_from_pool->get_address() << ": | ";

    std::array<type::byte, 4> byte_arr;
    byte_arr[0] = literal_from_pool->get_value() & 0x000000FF;
    byte_arr[1] = (literal_from_pool->get_value() >> 8) & 0x000000FF;
    byte_arr[2] = (literal_from_pool->get_value() >> 16) & 0x000000FF;
    byte_arr[3] = (literal_from_pool->get_value() >> 24) & 0x000000FF;

    for (type::byte single_byte : byte_arr)
    {
      if (!mode)
        std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)single_byte << " | ";
      else if (mode == 1)
        std::cout << "0b" << std::setfill('0') << std::setw(8) << std::bitset<8>(single_byte) << " | ";

      if (++new_line_cnt2 % line_width == 0)
      {
        std::cout << std::endl;
      }
    }
  }

  std::cout << std::endl
            << "Relocations: " << std::endl;
  for (RelocationRecord *rel : this->relocations)
  {
    std::cout << "Offset: " << rel->get_offset()
              << " | Addend: " << rel->get_addend()
              << " | Sym_id: " << rel->get_symbol_id()
              << " | Addend sign: " << rel->get_addend_signed_flag()
              << std::endl;
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
}

// adds literal to pool and set it's address
void Section::literal_pool_insert_new(LiteralPoolRecord *record)
{
  LiteralPoolKey key(record->get_value(), record->get_relocation_flag());

  // check if record already exists
  if (this->literal_pool.find(key) != this->literal_pool.end())
  {
    *record = *(this->literal_pool[key]);
    return;
  }

  record->set_address(this->get_length() + this->literal_pool.size() * 4); // XXX: check if 4 should be hardcoded
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

void Section::add_new_relocation(RelocationRecord *rel_record)
{
  this->relocations.insert(rel_record);
}

std::list<RelocationRecord *> Section::get_all_relocations() const
{
  std::list<RelocationRecord *> ret_list;

  for (RelocationRecord *rel : this->relocations)
    ret_list.push_back(rel);

  return ret_list;
}
