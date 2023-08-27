#include "../inc/literal.hpp"

Literal::Literal() : Parameter(type::PARAMETER_TYPE::LITERAL) {}
Literal::Literal(uint32_t val) : Parameter(type::PARAMETER_TYPE::LITERAL) { this->num_value = val; }
Literal::Literal(std::string val) : Parameter(type::PARAMETER_TYPE::LITERAL) { this->str_value = val; }
Literal::Literal(char *val) : Parameter(type::PARAMETER_TYPE::LITERAL) { this->str_value.assign(val); }

uint32_t Literal::get_num_value() const { return this->num_value; }
std::string Literal::get_str_value() const { return this->str_value; }

void Literal::set_num_value(uint32_t val) { this->num_value = val; }
void Literal::set_str_value(std::string val) { this->str_value = val; }
void Literal::set_str_value(char *val) { this->str_value.assign(val); }

Literal *Literal::clone() const { return new Literal(*this); }

bool Literal::set_id(int32_t id) { return false; }
int32_t Literal::get_id() { return -1; }