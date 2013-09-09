#ifndef __jg_new_line_parser__
#define __jg_new_line_parser__

#include <functional>
#include <vector>
#include <cstring>
#include <boost/mpl/string.hpp>
#include "util.h"

#define STATIC_STRING(...) \
  boost::mpl::c_str< boost::mpl::string<__VA_ARGS__> >
//---------------------------------------------------------
namespace jg {
namespace evented {
//---------------------------------------------------------

template<const char* delimiter>
class DelimiterParser {
public:
  typedef std::function<void(const char*, int)> Callback;

  DelimiterParser(Callback bound) : callback(bound) {}
  ~DelimiterParser() {}

  void feed(char* buf, int bufsz);

private:
  Callback          callback;
  std::vector<char> leftover;
};

template<const char* delimiter>
void DelimiterParser<delimiter>::feed(char* buffer, int bufsz) {
  int pos = 0;
  int len = strlen(delimiter);
  for (int i = 0; i <= bufsz - len; ++i) {
    if (strncmp(buffer + i, delimiter, len) == 0) {
      leftover.insert(leftover.end(), buffer + pos, buffer + i);
      callback(&leftover[0], leftover.size());
      leftover.resize(0);
      pos = i + len;
    }
  }

  if (pos < bufsz) {
    leftover.insert(leftover.end(), buffer + pos, buffer + bufsz);
    // DEBUG("Leftover" << std::string(&leftover[0], leftover.size()));
  }
}

typedef DelimiterParser<STATIC_STRING('\r', '\n')::value> CRLFParser;
//---------------------------------------------------------
} // end of namespace evented
} // end of namespace jg
//---------------------------------------------------------

#endif
