/*
Junegunn Choi (junegunn.c@gmail.com)
# 2013/09/08-
*/

#include <map>
#include <sstream>
#include "server.h"
#include "delimiter_parser.h"
#include "util.h"

using namespace std;

template<class Server, class Stream>
class ChatHandler
{
public:
  ChatHandler() : client_id(0) {}

  void on_open(Stream* stream) {
    id_map[stream] = ++client_id;

    ostringstream os;
    os << "<< Cliend #" << client_id << " has joined >>";
    publish(stream, os);
  }

  void on_data(Stream* stream, const char* data, int size) {
    ostringstream os;
    os << "[Client #" << id_map[stream] << "] " << string(data, size);
    publish(stream, os);
  }

  void on_close(Stream* stream) {
    ostringstream os;
    os << "<< Cliend #" << id_map[stream] << " has left >>";
    publish(stream, os);
  }

private:
  void publish(Stream* self, const ostringstream& data) {
    // http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
    for (Stream* stream : static_cast<Server*>(this)->streams()) {
      if (stream != self) {
        stream->write(data.str().c_str(), data.str().size());
        stream->write("\n", 1);
      }
    }
  }

  int client_id;
  std::map<Stream*, int> id_map;
};

int main(int argc, char *argv[]) {
  int port = argc > 1 ? jg::util::from_string<int>(argv[1]) : 8000;
  jg::evented::Server<
    jg::evented::DelimiterParser<'\n'>,
    ChatHandler,
    1024
  > server(port);
  server.start();

  return 0;
}

