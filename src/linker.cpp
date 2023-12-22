#include <iostream>
#include <fstream>
#include "../inc/linker.hpp"

void Interval::set_start(uint32_t start)
{
  this->start = start;
}

void Interval::set_end(uint32_t end)
{
  this->end = end;
}

uint32_t Interval::get_start()
{
  return this->start;
}

uint32_t Interval::get_end()
{
  return this->end;
}

std::string Interval::get_interval_string()
{
  return "[" + std::to_string(this->get_start()) + ", " + std::to_string(this->get_end()) + "]";
}

bool Interval::is_value_in_interval(uint32_t val)
{
  return val >= this->start && val <= this->end;
}

bool Interval::is_interval_valid()
{
  return this->start <= this->end;
}

Linker &Linker::get_instance()
{
  static Linker linker_instance;
  return linker_instance;
}

Linker::Linker()
{
  // TODO
}

Linker::~Linker()
{
  // TODO
}

void Linker::internal_error(std::string err_msg)
{
  this->internal_err = true;
  std::cerr << "[LINKER]: { INTERNAL ERROR: " << err_msg << " }" << std::endl;
}

void Linker::set_output_file_name(std::string file_name)
{
  this->output_file_name = file_name;
}

bool Linker::load_input_obj_files(std::vector<std::string> input_file_names)
{
  for (std::string file_name : input_file_names)
  {
    std::ifstream file_handle(file_name);

    if (!file_handle)
    {
      this->internal_error("Cannot read from obj file: " + file_name);
      return false;
    }

    std::string json_content = "", line;
    while (std::getline(file_handle, line))
    {
      json_content += line + "\n";
    }

    // remove the last new line
    json_content = json_content.substr(0, json_content.size() - 1);

    file_handle.close();
    ObjectFile *obj_file = new ObjectFile(json_content);
    obj_file->create_lookup_maps();
    this->obj_files.push_back(obj_file);
  }

  return true;
}

void Linker::set_section_places(std::unordered_map<std::string, uint32_t> section_places)
{
  this->section_places = section_places;
}

std::unordered_map<std::string, uint32_t> Linker::get_section_places()
{
  return section_places;
}

bool Linker::add_placed_section(std::string section_name, Interval addr_int)
{
  if (!addr_int.is_interval_valid())
    return false;

  if (this->placed_sections.find(section_name) == this->placed_sections.end())
  {
    // section is not already placed
    this->placed_sections[section_name] = addr_int;
    return true;
  }
  else
  {
    // section already placed, increase interval
    Interval old_addr_int = this->placed_sections[section_name];

    // sections need to be placed continuosly
    if (addr_int.get_start() != old_addr_int.get_end() + 1)
      return false;

    // increase old interval
    old_addr_int.set_end(addr_int.get_end());

    // update value in map
    this->placed_sections[section_name] = old_addr_int;
    return true;
  }
}

std::string Linker::get_placed_section_by_addr(uint32_t addr)
{
  for (auto iter : this->placed_sections)
  {
    std::string section_name = iter.first;
    Interval addr_int = iter.second;

    if (addr_int.is_value_in_interval(addr))
      return section_name;
  }

  return "";
}

uint32_t Linker::generate_next_id()
{
  return this->symbol_id++;
}

SymbolJsonRecord Linker::create_new_section(std::string section_name, uint32_t addr)
{
  SymbolJsonRecord section;
  section.set_id(this->generate_next_id());
  section.set_is_final(true);
  section.set_is_global(true);
  section.set_name(section_name);
  section.set_type(type::PARAMETER_TYPE::SECTION);
  section.set_value(addr);

  return section;
}

std::string Linker::create_hex()
{
  // TODO: implement creation of sections of final symbol table
  uint32_t max_mem_addr = 0;

  for (auto iter : this->section_places)
  {
    std::string section_name = iter.first;
    uint32_t start_mem_addr = iter.second;

    for (ObjectFile *obj_file : this->obj_files)
    {
      SectionJsonRecord *section = obj_file->get_section(section_name);
      if (!section)
        continue;

      // mark start addr of section
      Interval section_mem_interval(start_mem_addr);
      section->set_start_mem_addr(start_mem_addr);
      std::vector<type::byte> output_file = section->get_output_file();

      for (type::byte byte : output_file)
      {
        if (this->output_file_byte.find(start_mem_addr) != this->output_file_byte.end())
        {
          std::string already_placed_section = this->get_placed_section_by_addr(start_mem_addr);
          this->internal_error("Overlapping of two sections: \n\tsection 1: " + section_name + "\n\tsection 2: " + already_placed_section);
          return "";
        }

        this->output_file_byte[start_mem_addr] = byte;
        start_mem_addr++;
      }

      // mark end addr of section
      section_mem_interval.set_end(start_mem_addr - 1);

      // insert section to placed_sections at it's interval
      if (this->add_placed_section(section_name, section_mem_interval))
      {
        if (this->placed_sections.find(section_name) != this->placed_sections.end())
        {
          Interval old_interval = this->placed_sections[section_name];
          this->internal_error("Error adding section at interval: " + section_mem_interval.get_interval_string() +
                               ". Last interval: " + old_interval.get_interval_string());
          return "";
        }
        else
        {
          this->internal_error("Error adding section at interval: " + section_mem_interval.get_interval_string());
          return "";
        }
      }
    }

    // check if section is placed and if it is add it to global_sym_table
    if (start_mem_addr > iter.second)
    {
      // TODO: check addr value
      SymbolJsonRecord section = this->create_new_section(section_name, iter.second);
      this->global_sym_table[section_name] = section;
    }

    // update maximum free address
    if (start_mem_addr > max_mem_addr && start_mem_addr > iter.second)
      max_mem_addr = start_mem_addr;
  }

  uint32_t mem_cnt = max_mem_addr;
  for (ObjectFile *obj_file : this->obj_files)
  {
    for (auto section : obj_file->get_sections())
    {
      if (section.get_is_section_placed())
        continue;

      SymbolJsonRecord *symbol = obj_file->get_symbol(section.get_id());
      if (!symbol)
      {
        this->internal_error("Symbol cannot be fetched: " + std::to_string(section.get_id()));
        return "";
      }

      // use section_name to fetch section in other obj files
      std::string section_name = symbol->get_name();

      // check if there is any section with same name already placed ==> if so, return an error
      if (this->placed_sections.find(section_name) != this->placed_sections.end())
      {
        this->internal_error("Section should be already placed. section: " + section_name);
        return "";
      }

      for (ObjectFile *nested_obj_file : this->obj_files)
      {
        SectionJsonRecord *section_ptr = nested_obj_file->get_section(section_name);
        if (!section_ptr)
          continue;

        // mark start addr of section
        Interval section_mem_interval(mem_cnt);
        section_ptr->set_start_mem_addr(mem_cnt);
        std::vector<type::byte> output_file = section_ptr->get_output_file();

        for (type::byte byte : output_file)
        {
          if (this->output_file_byte.find(mem_cnt) != this->output_file_byte.end())
          {
            std::string already_placed_section = this->get_placed_section_by_addr(mem_cnt);
            this->internal_error("Overlapping of two sections: \n\tsection 1: " + section_name + "\n\tsection 2: " + already_placed_section);
            return "";
          }

          this->output_file_byte[mem_cnt] = byte;
          mem_cnt++;
        }

        // mark end addr of section
        section_mem_interval.set_end(mem_cnt - 1);

        // insert section to placed_sections at it's interval
        if (this->add_placed_section(section_name, section_mem_interval))
        {
          if (this->placed_sections.find(section_name) != this->placed_sections.end())
          {
            Interval old_interval = this->placed_sections[section_name];
            this->internal_error("Error adding section at interval: " + section_mem_interval.get_interval_string() +
                                 ". Last interval: " + old_interval.get_interval_string());
            return "";
          }
          else
          {
            this->internal_error("Error adding section at interval: " + section_mem_interval.get_interval_string());
            return "";
          }
        }
      }

      // check if section is placed and if it is add it to global_sym_table
      if (this->placed_sections.find(section_name) != this->placed_sections.end())
      {
        // TODO: check addr value
        SymbolJsonRecord section = this->create_new_section(section_name, this->placed_sections[section_name].get_start());
        this->global_sym_table[section_name] = section;
      }
    }
  }

  // TODO: combine all symbol tables to one (with recalculating offsets)

  // TODO: combine all relocation lists to one global (with recalculating offsets & addends)

  // TODO: resolve all relocation records
}

std::string Linker::create_relocatable()
{
  // TODO
}
