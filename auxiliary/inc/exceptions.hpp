#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>

class file_exception: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Specified file is not valid";
  }
};


#endif