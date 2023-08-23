#include "../inc/all_directives.hpp"
#include "../inc/symbol.hpp"
#include "../inc/literal.hpp"
#include <iostream>

directive::GLOBAL::GLOBAL()
{ // TODO: implement constructor
}

void directive::GLOBAL::execute() const
{ // TODO: implement GLOBAL execute
  std::cout << "-----------------------------" << std::endl;
  std::cout << "GLOBAL DIRECTIVE EXECUTED. Symbol list: " << std::endl;

  for (Parameter *const &param : this->params)
  {
    Symbol *symbol = (Symbol *)param;
    std::cout << symbol->get_name() << std::endl;
  }

  std::cout << "-----------------------------" << std::endl;
}

directive::EXTERN::EXTERN()
{ // TODO: implement constructor
}

void directive::EXTERN::execute() const
{ // TODO: implement EXTERN execute
  std::cout << "-----------------------------" << std::endl;
  std::cout << "EXTERN DIRECTIVE EXECUTED. Symbol list: " << std::endl;

  for (Parameter *const &param : this->params)
  {
    Symbol *symbol = (Symbol *)param;
    std::cout << symbol->get_name() << std::endl;
  }

  std::cout << "-----------------------------" << std::endl;
}

directive::SECTION::SECTION()
{ // TODO: implement constructor
}

void directive::SECTION::execute() const
{
  // TODO: implement SECTION execute
}

directive::WORD::WORD()
{ // TODO: implement constructor
}

void directive::WORD::execute() const
{
  // TODO: implement WORD execute
}

directive::SKIP::SKIP()
{ // TODO: implement constructor
}

void directive::SKIP::execute() const
{
  // TODO: implement SKIP execute
}

directive::ASCII::ASCII()
{ // TODO: implement constructor
}

void directive::ASCII::execute() const
{
  // TODO: implement ASCII execute
}

directive::END::END()
{ // TODO: implement constructor
}

void directive::END::execute() const
{
  // TODO: implement END execute
}