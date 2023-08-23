#include "../inc/section.hpp"

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

unsigned int Section::get_curr_loc_cnt() const
{
  return this->location_counter;
}

unsigned int Section::get_length() const
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

void Section::write_byte_arr(std::vector<type::byte> arr)
{
  for (type::byte single_byte : arr)
  {
    this->output_file.push_back(single_byte);
    this->location_counter++;
  }
}

void Section::write_byte(type::byte single_byte)
{
  this->output_file.push_back(single_byte);
  this->location_counter++;
}