#ifndef __jg_pass_parser__
#define __jg_pass_parser__

#include <functional>
#include "util.h"

//---------------------------------------------------------
namespace jg {
namespace evented {
//---------------------------------------------------------

class PassParser {
public:
  typedef std::function<void(const char*, int)> Callback;

  PassParser(Callback bound) : callback(bound) {}
  ~PassParser() {}

  void feed(char* buf, int bufsz) {
    callback(buf, bufsz);
  }

private:
  Callback callback;
};

//---------------------------------------------------------
} // end of namespace evented
} // end of namespace jg
//---------------------------------------------------------

#endif
