/*
Junegunn Choi (junegunn.c@gmail.com)
# 2013/09/08-
*/

#include "server.h"
#include "pass_parser.h"
#include "handler.h"
#include "util.h"

using namespace std;

int main(int argc, char *argv[]) {
  int port = argc > 1 ? jg::util::from_string<int>(argv[1]) : 8000;
  jg::evented::Server<
    jg::evented::PassParser,
    jg::evented::Handler,
    1024
  > server(port);
  server.start();

  return 0;
}

