#ifndef RELOCATION_RECORD_HPP
#define RELOCATION_RECORD_HPP

#include "types.hpp"
#include <functional>
#include <iomanip>

class RelocationRecord
{
private:
  uint32_t offset;
  int32_t symbol_id;
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

  bool operator==(const RelocationRecord &rel_record) const;
};

struct RelocationRecordHasher
{
  std::size_t operator()(RelocationRecord *const &rel) const
  {
    using std::hash;
    using std::size_t;

    // Start with a hash value of 0
    std::size_t seed = 0;

    // combine all hashes using inspiration from boost library
    seed ^= hash<uint32_t>()(rel->get_offset()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash<int32_t>()(rel->get_symbol_id()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash<int32_t>()(rel->get_addend()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash<uint8_t>()(static_cast<uint8_t>(rel->get_type())) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash<bool>()(rel->get_addend_signed_flag()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    // Return the result.
    return seed;
  }
};

struct PointedObjEq
{
  bool operator()(RelocationRecord const *rel1, RelocationRecord const *rel2) const
  {
    return rel1->get_offset() == rel2->get_offset() && rel1->get_type() == rel2->get_type() && rel1->get_symbol_id() == rel2->get_symbol_id() && rel1->get_addend() == rel2->get_addend();
  }
};

#endif