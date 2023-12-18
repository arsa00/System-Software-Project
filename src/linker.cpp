#include <iostream>
#include <fstream>
#include "../inc/linker.hpp"

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

      section->set_start_mem_addr(start_mem_addr);
      std::vector<type::byte> output_file = section->get_output_file();

      for (type::byte byte : output_file)
      {
        if (this->output_file_byte.find(start_mem_addr) != this->output_file_byte.end())
        {
          // TODO: get which section is located at this mem addr
          this->internal_error("Overlapping of two sections: \n\tsection 1: " + section_name + "\n\tsection 2: "); // add here name of other section
        }

        this->output_file_byte[start_mem_addr] = byte;
        start_mem_addr++;
      }
    }

    if (start_mem_addr > max_mem_addr)
      max_mem_addr = start_mem_addr;
  }

  uint32_t mem_cnt = max_mem_addr;
  for (ObjectFile *obj_file : this->obj_files)
  {
    for (auto section : obj_file->get_sections())
    {
      if (section.get_is_section_placed())
        continue;

      for (ObjectFile *nested_obj_file : this->obj_files)
      {
        SymbolJsonRecord *symbol = nested_obj_file->get_symbol(section.get_id());
        if (!symbol)
        {
          this->internal_error("Symbol cannot be fetched: " + std::to_string(section.get_id()));
          return "";
        }

        SectionJsonRecord *section_ptr = nested_obj_file->get_section(symbol->get_name());
        if (!section_ptr)
        {
          this->internal_error("Section cannot be fetched: " + symbol->get_name());
          return "";
        }

        section_ptr->set_start_mem_addr(mem_cnt);
        std::vector<type::byte> output_file = section_ptr->get_output_file();

        for (type::byte byte : output_file)
        {
          if (this->output_file_byte.find(mem_cnt) != this->output_file_byte.end())
          {
            // TODO: get which section is located at this mem addr
            this->internal_error("Overlapping of two sections: \n\tsection 1: " + symbol->get_name() + "\n\tsection 2: "); // add here name of other section
          }

          this->output_file_byte[mem_cnt] = byte;
          mem_cnt++;
        }
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
