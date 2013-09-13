/*
Junegunn Choi (junegunn.c@gmail.com)
# 2013/09/08-
*/

#include "server.h"
#include "delimiter_parser.h"
#include "util.h"

using namespace std;
using namespace jg;

template<class Server, class Stream>
class SeqHandler
{
public:
  SeqHandler() : counter(0), prev_counter(0) {}
  void on_open(Stream* stream) {}
  void on_close(Stream* stream) {}
  void on_data(Stream* stream, const char* data, int size) {
    int diff = 1;
    if (size > 0) {
      // RESET
      if (data[0] == '!') {
        counter = 0;
      } else {
        diff = util::from_string<int>(string(data, size));
        if (diff == 0) diff = 1;
      }
    }
    counter += diff;

    ostringstream os;
    os << counter << "\r\n";

    stream->write(os);
  }

  void print_stats(int interval) {
    if (counter > prev_counter)
      DEBUG((counter - prev_counter) / interval << " incr/sec");
    prev_counter = counter;
  }

private:
  long long int counter;
  long long int prev_counter;
};

int main(int argc, char *argv[]) {
  int port = argc > 1 ? jg::util::from_string<int>(argv[1]) : 8000;
  jg::evented::Server<
    jg::evented::CRLFParser,
    SeqHandler,
    1024
  > server(port);
  server.set_interval(10, [&server]() { server.print_stats(10); });
  server.start();

  return 0;
}

