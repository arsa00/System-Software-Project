#ifndef POOL_LITERAL_RECORD_HPP
#define POOL_LITERAL_RECORD_HPP

#include "types.hpp"
#include <functional>
#include <iomanip>

struct LiteralPoolKey
{
  uint32_t literal_value;
  bool is_record_relocatable = false;

  LiteralPoolKey(uint32_t value, bool realocation_flag = false) : literal_value(value), is_record_relocatable(realocation_flag) {}

  bool operator==(const LiteralPoolKey &key) const
  {
    return key.literal_value == this->literal_value && key.is_record_relocatable == this->is_record_relocatable;
  }
};

struct LiteralPoolKeyHasher
{
  std::size_t operator()(const LiteralPoolKey &k) const
  {
    using std::hash;
    using std::size_t;

    return hash<uint32_t>()(k.literal_value) ^ (hash<bool>()(k.is_record_relocatable) << 1);
  }
};

class LiteralPoolRecord
{
private:
  uint32_t literal_value;
  bool is_relocatable;
  type::addr_size address;
  // uint8_t size = 4; // XXX: probably not needed, because all literals will be 32bit long

public:
  LiteralPoolRecord() = default;
  LiteralPoolRecord(uint32_t value, bool realocation_flag = false);

  void set_value(uint32_t value);
  uint32_t get_value() const;

  void set_address(type::addr_size addr);
  type::addr_size get_address() const;

  void set_relocation_flag(bool flag_value);
  bool get_relocation_flag() const;
};

#endif