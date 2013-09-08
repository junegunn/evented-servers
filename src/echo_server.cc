/*
Junegunn Choi (junegunn.c@gmail.com)
# 2013/09/08-
*/

#include "server.h"
#include "delimiter_parser.h"
#include "util.h"

using namespace std;

template<class Server, class Stream>
class EchoHandler
{
public:
  EchoHandler() {}

  void on_open(Stream* stream) {
    DEBUG("Stream opened");
  }

  void on_data(Stream* stream, const char* data, int size) {
    stream->write(data, size);
    stream->write("\r\n", 2);
  }

  void on_close(Stream* stream) {
    DEBUG("Stream closed");
  }
};

int main(int argc, char *argv[]) {
  int port = argc > 1 ? jg::util::from_string<int>(argv[1]) : 8000;
  jg::evented::Server<
    jg::evented::CRLFParser,
    EchoHandler,
    1024
  > server(port);
  server.start();

  return 0;
}

