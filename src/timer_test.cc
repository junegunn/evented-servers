/*
Junegunn Choi (junegunn.c@gmail.com)
# 2013/09/08-
*/

#include <unistd.h>
#include "server.h"
#include "pass_parser.h"
#include "handler.h"
#include "util.h"

using namespace std;
using namespace jg;

int main(int argc, char *argv[]) {
  int port = argc > 1 ? jg::util::from_string<int>(argv[1]) : 8000;
  jg::evented::Server<
    jg::evented::PassParser,
    jg::evented::Handler,
    1024
  > server(port);
  auto local = "Interval";
  server.set_timeout(3, []() { cout << "Timer" << endl; });
  server.set_interval(1, [&](void) -> void { cout << local << endl; });
  server.start();

  return 0;
}

