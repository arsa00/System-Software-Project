#include "../inc/literal_pool_record.hpp"

LiteralPoolRecord::LiteralPoolRecord(uint32_t value, bool relocation_flag) : literal_value(value), is_relocatable(relocation_flag) {}

void LiteralPoolRecord::set_value(uint32_t value) { this->literal_value = value; }
uint32_t LiteralPoolRecord::get_value() const { return this->literal_value; }

void LiteralPoolRecord::set_address(type::addr_size addr) { this->address = addr; }
type::addr_size LiteralPoolRecord::get_address() const { this->address; }

void LiteralPoolRecord::set_relocation_flag(bool flag_value) { this->is_relocatable = flag_value; }
bool LiteralPoolRecord::get_relocation_flag() const { return this->is_relocatable; }