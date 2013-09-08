#ifndef __jg_new_line_parser__
#define __jg_new_line_parser__

#include <functional>
#include <vector>
#include "util.h"

//---------------------------------------------------------
namespace jg {
namespace evented {
//---------------------------------------------------------

template<char delimiter>
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

template<char delimiter>
void DelimiterParser<delimiter>::feed(char* buffer, int bufsz) {
  int pos = 0;
  for (int i = 0; i < bufsz; ++i) {
    if (buffer[i] == delimiter) {
      leftover.insert(leftover.end(), buffer + pos, buffer + i);
      callback(&leftover[0], leftover.size());
      leftover.resize(0);
      pos = i + 1;
    }
  }

  if (pos < bufsz) {
    leftover.insert(leftover.end(), buffer + pos, buffer + bufsz);
    // DEBUG("Leftover" << std::string(&leftover[0], leftover.size()));
  }
}
//---------------------------------------------------------
} // end of namespace evented
} // end of namespace jg
//---------------------------------------------------------

#endif
