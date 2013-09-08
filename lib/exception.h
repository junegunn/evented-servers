#ifndef __jg_exception__
#define __jg_exception__

#include <iostream>

//---------------------------------------------------------
namespace jg {
//---------------------------------------------------------

class Exception : public std::exception
{
public:
  Exception() {}
  Exception(const std::string& msg) : msg(msg) {}
  Exception(int errno) throw() : msg(strerror(errno)) {}
  virtual ~Exception() throw() {}

  const char* what() const throw() { return msg.c_str(); }
  void print() { std::cerr << msg << std::endl; }
private:
  std::string msg;
};

//---------------------------------------------------------
} // end of namespace jg
//---------------------------------------------------------

#endif
