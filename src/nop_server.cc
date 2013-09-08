/*
Junegunn Choi (junegunn.c@gmail.com)
# 2013/09/08-
*/

#include "server.h"
#include "pass_parser.h"
#include "util.h"

using namespace std;

template<class Server, class Stream>
class NopHandler
{
public:
  NopHandler() {}
  void on_open(Stream* stream) {}
  void on_data(Stream* stream, const char* data, int size) {}
  void on_close(Stream* stream) {}
};

int main(int argc, char *argv[]) {
  int port = argc > 1 ? jg::util::from_string<int>(argv[1]) : 8000;
  jg::evented::Server<
    jg::evented::PassParser,
    NopHandler,
    1024
  > server(port);
  server.start();

  return 0;
}

