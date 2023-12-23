#include <iostream>
#include <fstream>
#include <algorithm>
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

void Linker::add_sym_to_table(SymbolJsonRecord sym)
{
  SymbolTableKey sym_key(sym.get_name(), sym.get_type());
  this->global_sym_table[sym_key] = sym;
  this->sym_table_lookup_map[sym.get_id()] = sym_key;
}

bool Linker::is_sym_in_table(std::string sym_name, type::PARAMETER_TYPE sym_type)
{
  SymbolTableKey sym_key(sym_name, sym_type);
  if (this->global_sym_table.find(sym_key) == this->global_sym_table.end())
    return false;

  return true;
}

bool Linker::is_sym_in_table(SymbolJsonRecord sym)
{
  return this->is_sym_in_table(sym.get_name(), sym.get_type());
}

bool Linker::is_sym_in_table(uint32_t sym_id)
{
  if (this->sym_table_lookup_map.find(sym_id) == this->sym_table_lookup_map.end())
    return false;

  if (this->global_sym_table.find(this->sym_table_lookup_map[sym_id]) == this->global_sym_table.end())
    return false;

  return true;
}

SymbolJsonRecord *Linker::get_sym_from_table(std::string sym_name, type::PARAMETER_TYPE sym_type)
{
  if (!this->is_sym_in_table(sym_name, sym_type))
    return nullptr;

  return &this->global_sym_table[SymbolTableKey(sym_name, sym_type)];
}

SymbolJsonRecord *Linker::get_sym_from_table(SymbolJsonRecord sym)
{
  return this->get_sym_from_table(sym.get_name(), sym.get_type());
}

SymbolJsonRecord *Linker::get_sym_from_table(uint32_t sym_id)
{
  if (!this->is_sym_in_table(sym_id))
    return nullptr;

  return &this->global_sym_table[this->sym_table_lookup_map[sym_id]];
}

// always check for internal error after calling this method
std::vector<type::byte> Linker::resolve_relocation(RelocationJsonRecord relocation)
{
  if (!this->is_sym_in_table(relocation.get_sym_id()))
  {
    this->internal_error("Symbol is missing in linker symbol talbe. symbol_id: " + std::to_string(relocation.get_sym_id()));
    return {};
  }

  SymbolJsonRecord *sym = this->get_sym_from_table(relocation.get_sym_id());
  if (!sym)
  {
    this->internal_error("Symbol cannot be fethced from symbol talbe. symbol_id: " + std::to_string(relocation.get_sym_id()));
    return {};
  }

  std::vector<type::byte> result_bytes;

  // get symbol offset/addr
  int32_t sym_value;
  if (!sym->get_value(&sym_value))
  {
    this->internal_error("Symbol is not resolved (it doesn't have value). symbol_name: " + sym->get_name() + ", symbol_id: " + std::to_string(sym->get_id()));
    return {};
  }

  // calculate starting memory offset/addr
  uint32_t start_mem_offset = sym_value;

  if (relocation.get_is_addend_signed())
  {
    int32_t addend = (int32_t)relocation.get_addend();
    start_mem_offset += addend;
  }
  else
  {
    start_mem_offset += relocation.get_addend();
  }

  uint32_t bytes_to_read = 0;
  if (relocation.get_type() == type::RELOCATIONS::ABS_32S || relocation.get_type() == type::RELOCATIONS::ABS_32U)
    bytes_to_read = 4;

  for (uint32_t i = 0; i < bytes_to_read; i++)
  {
    if (this->output_file_byte.find(start_mem_offset + i) == this->output_file_byte.end())
    {
      // no data at given offset
      this->internal_error("There's no data at calculated offset. offset: " + std::to_string(start_mem_offset + i) + ", relocation_json: " + relocation.convert_to_json());
      return {};
    }

    result_bytes.push_back(this->output_file_byte[start_mem_offset + i]);
  }

  return result_bytes;
}

std::string Linker::create_hex()
{
  // mapping sections
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
      this->add_sym_to_table(section);
      // this->global_sym_table[section_name] = section;
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
        this->add_sym_to_table(section);
        // this->global_sym_table[section_name] = section;
      }
    }
  }

  // combine all symbol tables to one (with recalculating offsets)
  for (ObjectFile *obj_file : this->obj_files)
  {
    std::vector<SymbolJsonRecord> sym_table = obj_file->get_symbol_table();
    for (SymbolJsonRecord sym : sym_table)
    {
      // no need to include local symbols
      if (!sym.get_is_global())
        continue;

      // sections that contain data should've already beeen added to sym_table
      if (sym.get_type() == type::PARAMETER_TYPE::SECTION)
      {
        SectionJsonRecord *section = obj_file->get_section(sym.get_name());
        if (!section)
        {
          this->internal_error("Section cannot be fetched. section_name: " + sym.get_name());
          return "";
        }

        // this->global_sym_table.find(sym.get_name()) == this->global_sym_table.end()
        if (!this->is_sym_in_table(sym) && section->get_output_file().size() > 0)
        {
          this->internal_error("Section is missing in symbol table of linker. section_name: " + sym.get_name());
          return "";
        }

        continue;
      }

      // this->global_sym_table.find(sym.get_name()) != this->global_sym_table.end()
      if (this->is_sym_in_table(sym))
      {
        this->internal_error("Multiple definitions of symbol. symbol_name: " + sym.get_name());
        return "";
      }

      uint32_t sym_section_id;
      if (sym.get_section(&sym_section_id))
      {
        SymbolJsonRecord *section_sym = obj_file->get_symbol(sym_section_id);
        if (!section_sym)
        {
          this->internal_error("Symbol cannot be fetched: " + std::to_string(sym_section_id));
          return "";
        }

        SectionJsonRecord *section = obj_file->get_section(section_sym->get_name());
        if (!section)
        {
          this->internal_error("Section cannot be fetched. section_name: " + section_sym->get_name());
          return "";
        }

        if (!section->get_is_section_placed())
        {
          this->internal_error("Section is not placed. section_name " + section_sym->get_name());
          return "";
        }

        // this->global_sym_table.find(section_sym->get_name()) == this->global_sym_table.end()
        if (!this->is_sym_in_table(*section_sym))
        {
          this->internal_error("Section is missing in symbol table of linker. section_name: " + section_sym->get_name());
          return "";
        }

        int32_t sym_value;
        if (!sym.get_value(&sym_value))
        {
          this->internal_error("Symbol is not resolved (it doesn't have value). symbol_name: " + sym.get_name() + ", symbol_id: " + std::to_string(sym.get_id()));
          return "";
        }

        sym.set_value(sym_value + section->get_start_mem_addr());
        sym.set_section(this->global_sym_table[section_sym->get_name()].get_id());
        sym.set_id(this->generate_next_id());
        sym.set_is_final(true);
        this->add_sym_to_table(sym);
        // this->global_sym_table[sym.get_name()] = sym;
      }
      else
      {
        this->internal_error("Symbol is missing section id. symbol_name: " + sym.get_name() + ", symbol_id: " + std::to_string(sym.get_id()));
        return "";
      }
    }
  }

  // combine all relocation lists to one global (with recalculating offsets & addends)
  for (ObjectFile *obj_file : this->obj_files)
  {
    for (SectionJsonRecord section : obj_file->get_sections())
    {
      if (!section.get_is_section_placed())
      {
        SymbolJsonRecord *sec_sym = obj_file->get_symbol(section.get_id());
        if (sec_sym)
        {
          this->internal_error("Section is not placed. section_name: " + sec_sym->get_name());
          return "";
        }

        this->internal_error("Section is not placed. section_id: " + section.get_id());
        return "";
      }

      for (RelocationJsonRecord relocation : section.get_relocations())
      {
        SymbolJsonRecord *sym = obj_file->get_symbol(relocation.get_sym_id());
        if (!sym)
        {
          this->internal_error("Symbol cannot be fetched. symbol_name: " + sym->get_name());
          return "";
        }

        // this->global_sym_table.find(sym->get_name()) == this->global_sym_table.end()
        if (!this->is_sym_in_table(*sym))
        {
          this->internal_error("Symbol is missing in symbol table of linker. symbol_name: " + sym->get_name());
          return "";
        }

        if (sym->get_type() == type::PARAMETER_TYPE::SECTION)
        {
          SectionJsonRecord *target_section = obj_file->get_section(sym->get_name());
          {
            this->internal_error("Section cannot be fetched. section_name: " + sym->get_name());
            return "";
          }

          if (!target_section->get_is_section_placed())
          {
            this->internal_error("Section is not placed. section_name: " + sym->get_name());
            return "";
          }

          int32_t global_sym_value;
          if (!this->global_sym_table[sym->get_name()].get_value(&global_sym_value))
          {
            this->internal_error("Symbol is not resolved (it doesn't have value). symbol_name: " + sym->get_name() + ", symbol_id: " + std::to_string(this->global_sym_table[sym->get_name()].get_id()));
            return "";
          }

          relocation.set_addend(relocation.get_addend() + target_section->get_start_mem_addr() - global_sym_value);
          relocation.set_sym_id(this->global_sym_table[sym->get_name()].get_id());
          relocation.set_offset(relocation.get_offset() + section.get_start_mem_addr());
          this->global_relocations.push_back(relocation);
        }
        else
        {
          relocation.set_sym_id(this->global_sym_table[sym->get_name()].get_id());
          relocation.set_offset(relocation.get_offset() + section.get_start_mem_addr());
          this->global_relocations.push_back(relocation);
        }
      }
    }
  }

  // resolve all relocation records
  for (RelocationJsonRecord relocation : this->global_relocations)
  {
    if (!relocation.is_addend_valid())
    {
      this->internal_error("Addend and type of relocation record are not valid. relocation_json: " + relocation.convert_to_json());
      return "";
    }

    std::vector<type::byte> bytes_to_write = this->resolve_relocation(relocation);
    if (this->internal_err)
      return "";

    uint32_t start_mem_offset = relocation.get_offset();
    for (uint32_t i = 0; i < bytes_to_write.size(); i++)
    {
      this->output_file_byte[start_mem_offset + i] = bytes_to_write[i];
    }
  }

  // create result output file
  // sort mem addrs in ASC order
  std::vector<uint32_t> mem_addrs(this->output_file_byte.size());

  for (auto iter : this->output_file_byte)
    mem_addrs.push_back(iter.first);

  std::sort(mem_addrs.begin(), mem_addrs.end());

  // get all bytes and create output string
  std::string output = "";
  const uint8_t output_width = 8;
  uint32_t cnt = 0;
  while (cnt < mem_addrs.size())
  {
    char addr_str[10];
    sprintf(addr_str, "%08X", mem_addrs[cnt]);
    output += std::string(addr_str) + ": ";

    uint32_t previous_addr = mem_addrs[cnt] - 1; // TODO: check this
    for (uint8_t j = 0; j < output_width; j++)
    {
      char value_str[3];

      if (mem_addrs[cnt] == previous_addr + 1)
      {
        sprintf(value_str, "%02X", this->output_file_byte[mem_addrs[cnt]]);
        output += std::string(value_str);
        cnt++;
      }
      else
      {
        output += "00";
      }

      if (j < output_width - 1)
        output += " ";

      previous_addr++; // TODO: check this
    }

    output += "\n";
  }

  return output;
}

std::string Linker::create_relocatable()
{
  // TODO
}
