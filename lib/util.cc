#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include "util.h"

void jg::util::check(bool assert, const char* msg) {
  if (!assert) {
    perror(msg);
    exit(1);
  }
}

