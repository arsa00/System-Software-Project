#ifndef RELOCATION_RECORD_HPP
#define RELOCATION_RECORD_HPP

#include "types.hpp"

class RelocationRecord
{
private:
  uint32_t offset;
  uint32_t symbol_id;
  int32_t addend;
  type::RELOCATIONS type;
  bool is_addend_signed;

  static bool get_signed_flag_from_type(type::RELOCATIONS type);

public:
  RelocationRecord(uint32_t offset, int32_t symbol_id, type::RELOCATIONS type, int32_t addend = 0);

  uint32_t get_offset() const;
  int32_t get_symbol_id() const;
  int32_t get_addend() const;
  type::RELOCATIONS get_type() const;
  bool get_addend_signed_flag() const;
};

#endif