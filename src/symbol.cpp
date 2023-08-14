#include "../inc/symbol.hpp"

uint32_t Symbol::get_value() const { return this->value; }
std::string Symbol::get_name() const { return this->name; }

void Symbol::set_value(uint32_t val) { this->value = val; }
void Symbol::set_name(std::string val) { this->name = val; }
void Symbol::set_name(char *val) { this->name.assign(val); }

Symbol* Symbol::clone() const { return new Symbol(*this); }