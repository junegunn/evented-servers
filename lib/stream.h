/*
Junegunn Choi (junegunn.c@gmail.com)
# 2013/09/04-
*/

#ifndef __jg_evented_stream__
#define __jg_evented_stream__

#include <vector>
#include <sstream>
#include <functional>
#include <ev++.h>
#include <fcntl.h>
#include "util.h"

//---------------------------------------------------------
namespace jg {
namespace evented {
//---------------------------------------------------------

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

template<class Parser, int buf_sz>
class Stream {
public:
  typedef std::function<void(Stream*, const char*, int)> DataCallback;
  typedef std::function<void(Stream*)> CloseCallback;

  Stream(int client_socket, DataCallback data_cb, CloseCallback close_cb)
  //: parser(std::bind(cb, this, _1, _2)) // For some reason, this doesn't compile
    : data_callback(data_cb),
      close_callback(close_cb),
      parser(std::bind(&Stream::on_data, this, _1, _2))
  {
    // Non-blocking
    fcntl(client_socket, F_SETFL, O_NONBLOCK);

    // Ignore SIGPIPE
    // http://pod.tst.eu/http://cvs.schmorp.de/libev/ev.pod#The_special_problem_of_SIGPIPE
    fcntl(client_socket, F_SETNOSIGPIPE, 1);

    watcher.set<Stream, &Stream::callback>(this);
    watcher.start(client_socket, ev::READ);
  }

  virtual ~Stream() {
    watcher.stop();
    shutdown(watcher.fd, SHUT_RDWR);
  }

  void write(const std::ostringstream& stream) {
    const std::string& str(stream.str());
    write(str.c_str(), str.size());
  }

  void write(const char* buffer, int sz) {
    write_buffer.insert(write_buffer.end(), buffer, buffer + sz);
    watcher.set(ev::READ | ev::WRITE);
  }

private:
  void callback(ev::io& watcher, int revents) {
    if (revents & ev::ERROR) {
      DEBUG("error");
      close_callback(this);
      return;
    }
    if ((revents & ev::READ) && !handle_read())   return;
    if ((revents & ev::WRITE) && !handle_write()) return;

    watcher.set(write_buffer.empty() ? ev::READ : (ev::READ | ev::WRITE));
  }

  void on_data(const char* buffer, int size) {
    data_callback(this, buffer, size);
  }

  bool handle_read() {
    char buf[buf_sz];
    int read_bytes = recv(watcher.fd, buf, buf_sz, 0);

    switch (read_bytes) {
      case -1:
        DEBUG("recv error");
        close_callback(this);
        return 0;
      case 0:
        DEBUG("closed");
        close_callback(this);
        return 0;
      default:
        parser.feed(buf, read_bytes);
        return 1;
    }
  }

  bool handle_write() {
    int written_bytes = send(watcher.fd, &write_buffer[0], write_buffer.size(), 0);
    if (written_bytes == -1) {
      DEBUG("send error");
      close_callback(this);
      return 0;
    } else {
      // Slice vector
      write_buffer.erase(
        write_buffer.begin(),
        write_buffer.begin() + written_bytes);
      return 1;
    }
  }

  DataCallback      data_callback;
  CloseCallback     close_callback;
  Parser            parser;
  ev::io            watcher;
  std::vector<char> write_buffer;
};

//---------------------------------------------------------
} // end of namespace evented
} // end of namespace jg
//---------------------------------------------------------

#endif
