#include "../inc/relocation_record.hpp"
#include <iostream>

bool RelocationRecord::get_signed_flag_from_type(type::RELOCATIONS type)
{
  // in this switch stmt ALL signed types should be stated
  // body of each case shoud just return true
  switch (type)
  {
  case type::RELOCATIONS::ABS_32S:
    return true;
  }

  return false; // if none of switch cases executed, return false
}

RelocationRecord::RelocationRecord(uint32_t offset, int32_t symbol_id, type::RELOCATIONS type, int32_t addend)
{
  this->offset = offset;
  this->symbol_id = symbol_id;
  this->type = type;
  this->addend = addend;
  this->is_addend_signed = RelocationRecord::get_signed_flag_from_type(type);
}

uint32_t RelocationRecord::get_offset() const
{
  return this->offset;
}

int32_t RelocationRecord::get_symbol_id() const
{
  return this->symbol_id;
}

int32_t RelocationRecord::get_addend() const
{
  return this->addend;
}

type::RELOCATIONS RelocationRecord::get_type() const
{
  return this->type;
}

bool RelocationRecord::get_addend_signed_flag() const
{
  return this->is_addend_signed;
}

bool RelocationRecord::operator==(const RelocationRecord &rel_record) const
{
  return rel_record.offset == this->offset && rel_record.type == this->type && rel_record.symbol_id == this->symbol_id && rel_record.addend == this->addend;
}