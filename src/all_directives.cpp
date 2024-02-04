#include "../inc/all_directives.hpp"
#include "../inc/symbol.hpp"
#include "../inc/literal.hpp"
#include "../inc/assembler.hpp"
#include "../inc/converters.hpp"
#include <iostream>

directive::GLOBAL::GLOBAL()
{
  this->is_generating_data = false;
}

void directive::GLOBAL::execute(Section *dest_section) const
{ // TODO: test GLOBAL

  for (Parameter *const &param : this->params)
  {
    if (param->get_type() != type::PARAMETER_TYPE::SYMBOL)
    {
      Assembler::get_instance().internal_error("Only symbols are expected with the .global directive.");
      return;
    }
    Symbol *sym = (Symbol *)param;
    sym->set_global_flag(true);
  }
}

directive::EXTERN::EXTERN()
{
  this->is_generating_data = false;
}

void directive::EXTERN::execute(Section *dest_section) const
{ // TODO: test EXTERN
  for (Parameter *const &param : this->params)
  {
    if (param->get_type() != type::PARAMETER_TYPE::SYMBOL)
    {
      Assembler::get_instance().internal_error("Only symbols are expected with the .extern directive.");
      return;
    }
    Symbol *sym = (Symbol *)param;
    sym->set_global_flag(true);
    sym->set_defined_flag(true);
  }
}

directive::SECTION::SECTION()
{
  this->is_generating_data = false;
}

void directive::SECTION::execute(Section *dest_section) const
{ // TODO: test SECTION
  Parameter *param = this->params.front();
  if (param->get_type() != type::PARAMETER_TYPE::SYMBOL)
  {
    Assembler::get_instance().internal_error("Only symbols are expected with the .section directive.");
    return;
  }

  Assembler::get_instance().add_section(((Symbol *)param)->get_name());
}

directive::WORD::WORD()
{
  this->is_generating_data = true;
}

// needed to calculate size in virtual method, because it depends on
// number of parameters passed to dirtective
uint32_t directive::WORD::get_size() const
{
  // every value in param list (both symbols and literals) takes 4 bytes
  return this->params.size() * 4;
}

void directive::WORD::execute(Section *dest_section) const
{ // TODO: test WORD
  uint32_t val;
  std::array<type::byte, 4> val_to_write;

  for (Parameter *const &param : this->params)
  {
    val = 0;
    val_to_write = {0, 0, 0, 0};
    if (param->get_type() == type::PARAMETER_TYPE::SYMBOL)
    {
      Symbol *sym = (Symbol *)param;
      if (!sym->get_defined_flag())
      {
        Assembler::get_instance().internal_error("Using undefined symbol: \"" + sym->get_name() + std::string("\". Cannot generate .word directive."));
        return;
      }

      // XXX: maybe add logic for symbols declared with .equ directive (final_flag)
      // create relocation record for symbol (so that linker can update it's value), and add it to section's relocations list
      int32_t sym_id = sym->get_global_flag() ? sym->get_id() : sym->get_section()->get_id();
      uint32_t addend = sym->get_global_flag() ? 0 : sym->get_value();
      RelocationRecord *rel_record = new RelocationRecord(dest_section->get_curr_loc_cnt(), sym_id, type::RELOCATIONS::ABS_32U, addend); // XXX: check if current loc_cnt is correctly used
      dest_section->add_new_relocation(rel_record);
      val = sym->get_id();
    }
    else if (param->get_type() == type::PARAMETER_TYPE::LITERAL)
    {
      val = ((Literal *)param)->get_num_value();
    }
    else
    {
      Assembler::get_instance().internal_error("Wrong parameter type passed to the .word directive.");
      return; // XXX: maybe continue instead
    }

    val_to_write = converter::get_instruction_bytes(static_cast<type::instruction_size>(val));
    dest_section->write_byte_arr({val_to_write.begin(), val_to_write.end()});
  }
}

directive::SKIP::SKIP()
{
  this->is_generating_data = true;
}

// needed to calculate size in virtual method, because it depends on
// value of literal passed to dirtective
uint32_t directive::SKIP::get_size() const
{
  if (this->params.front()->get_type() != type::PARAMETER_TYPE::LITERAL)
    return 0;

  return ((Literal *)this->params.front())->get_num_value();
}

void directive::SKIP::execute(Section *dest_section) const
{ // TODO: implement SKIP execute
  if (this->params.front()->get_type() != type::PARAMETER_TYPE::LITERAL)
  {
    Assembler::get_instance().internal_error("Wrong parameter type passed to the .skip directive.");
    return;
  }

  uint32_t aloc_size = ((Literal *)this->params.front())->get_num_value();
  for (uint32_t i = 0; i < aloc_size; i++)
    dest_section->write_byte(0);
}

directive::ASCII::ASCII()
{
  this->is_generating_data = true;
}

// needed to calculate size in virtual method, because it depends on
// length of string literal passed to dirtective
uint32_t directive::ASCII::get_size() const
{
  if (this->params.front()->get_type() != type::PARAMETER_TYPE::LITERAL)
    return 0;

  return ((Literal *)this->params.front())->get_str_value().length() + 1; // +1 for '\0' character
}

void directive::ASCII::execute(Section *dest_section) const
{ // TODO: test ASCII
  Parameter *param = this->params.front();
  if (param->get_type() != type::PARAMETER_TYPE::LITERAL)
  {
    Assembler::get_instance().internal_error("Wrong parameter type passed to the .ascii directive.");
    return;
  }

  std::string str_value = ((Literal *)param)->get_str_value();
  for (uint32_t i = 0; i < str_value.length(); i++)
    dest_section->write_byte(static_cast<type::byte>(str_value[i]));

  dest_section->write_byte(0);
}

directive::END::END()
{
  this->is_generating_data = false;
}

void directive::END::execute(Section *dest_section) const
{ // TODO: test END
  Assembler::get_instance().stop();
}