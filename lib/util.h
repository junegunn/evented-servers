#ifndef __jg_util__
#define __jg_util__

#include <iostream>
#include <sstream>

//---------------------------------------------------------
namespace jg {
namespace util {
//---------------------------------------------------------

// http://stackoverflow.com/questions/8843604/c-string-c-str-deallocation-necessary
template<class T> std::string to_string(const T& val) {
  std::ostringstream ost;
  ost << val;
  return ost.str();
}

template<class T> T from_string(const std::string& val) {
  std::istringstream ist(val);
  T ret;
  ist >> ret;
  return ret;
}

void check(bool assert, const char* msg = "Error");

//---------------------------------------------------------
} // end of namespace util
} // end of namespace jg
//---------------------------------------------------------

#define DEBUG(msg) \
  std::cerr << "["  << __func__ << "@" << __FILE__ << ":" << __LINE__ \
            << "] " << msg << std::endl

#define DEBUGP \
  std::cerr << "["  << __func__ << "@" << __FILE__ << ":" << __LINE__ \
            << "] " << strerror(errno) << std::endl

#endif
