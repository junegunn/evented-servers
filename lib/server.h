/*
Junegunn Choi (junegunn.c@gmail.com)
# 2013/09/04-
*/

/*
 * References:
 * - http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
 * - http://pod.tst.eu/http://cvs.schmorp.de/libev/ev.pod
 *
 */

#ifndef __jg_evented_server__
#define __jg_evented_server__

#include <unordered_set>
#include <unordered_map>
#include <memory> // std::shared_ptr
#include <functional>
#include <ev++.h>
#include "addr_info.h"
#include "stream.h"
#include "util.h"

#define CHECK(expr, msg) util::check(expr != -1, msg)

//---------------------------------------------------------
namespace jg {
namespace evented {
//---------------------------------------------------------

// CRTP
template<class Parser, template<class, class> class Handler, int buf_sz>
class Server : public Handler<Server<Parser, Handler, buf_sz>, Stream<Parser, buf_sz>> {
public:
  typedef Stream<Parser, buf_sz> ClientStream;
  typedef std::function<void(void)> Callback;

  Server(int listen_port) : port(listen_port) {
    DEBUG("evented-servers " << VERSION);

    unsigned int supported   = ev::supported_backends();
    unsigned int recommended = ev::recommended_backends();
    unsigned int chosen      = loop.backend();
    #define INFO(TYPE) \
      DEBUG(#TYPE << " " << ((supported & ev::TYPE) > 0) \
                  << " -> " << ((recommended & ev::TYPE) > 0) \
                  << " -> " << ((chosen & ev::TYPE)> 0))
    INFO(SELECT);
    INFO(POLL);
    INFO(EPOLL);
    INFO(KQUEUE);
    #undef INFO
  }

  virtual ~Server() {
    shutdown(listener.fd, SHUT_RDWR);
    for (auto& pair : timers) {
      pair.first->stop();
      delete pair.first;
      delete pair.second;
    }
  }

  void start() {
    DEBUG("Listening on: " << port);
    ready(port);
    loop.run();
  }

  void set_timeout(const ev::tstamp& t, const Callback& callback) {
    set_timer<&Server::timeout>(t, callback);
  }

  void set_interval(const ev::tstamp& t, const Callback& callback) {
    set_timer<&Server::interval>(t, callback);
  }

  const std::unordered_set<ClientStream*>& streams() {
    return streams_;
  }

private:
  template<void (Server::*method)(ev::timer&, int)>
  void set_timer(const ev::tstamp& t, const Callback& callback) {
    ev::timer* timer        = new ev::timer;
    Callback*  callback_ptr = new Callback(callback);

    timers[timer] = callback_ptr;
    timer->set<Server, method>(this);
    timer->start(t, t);
  }

  void timeout(ev::timer& timer, int revents) {
    timer.stop();
    Callback* cb = timers[&timer];
    (*cb)();
    delete timers[&timer];
    timers.erase(&timer);
    delete &timer;
  }

  void interval(ev::timer& timer, int revents) {
    Callback* cb = timers[&timer];
    (*cb)();
  }

  void io_accept(ev::io& watcher, int revents) {
    // Designed to be large enough to hold both IPv4 and IPv6 structures
    struct sockaddr_storage caddr;
    socklen_t addr_len = sizeof(caddr);
    int client = accept(watcher.fd, (struct sockaddr*) &caddr, &addr_len);

    ClientStream* stream = new ClientStream(
        client,
        std::bind(&Server::on_data, this, _1, _2, _3),
        std::bind(&Server::close_stream, this, _1));
    this->on_open(stream);
    streams_.insert(stream);

    DEBUG("We have " << streams_.size() << " stream(s)");
  }

  void sig_int(ev::sig& watcher, int revents) {
    DEBUG("Interrupted");
    watcher.loop.break_loop();
  }

  void close_stream(ClientStream* stream) {
    this->on_close(stream);
    delete stream;
    streams_.erase(stream);
  }

  void ready(int port) {
    AddrInfo addr(port);

    // Create socket
    int fd;
    struct addrinfo* info = addr.info();
    fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    CHECK(fd, "Failed to create socket");

    // Bind
    int y = 1;
    CHECK(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y)), "setsockopt");
    CHECK(bind(fd, info->ai_addr, info->ai_addrlen), "bind");

    // Listen
    CHECK(listen(fd, 10), "listen");

    // Setup watcher for listening
    listener.set<Server, &Server::io_accept>(this);
    listener.start(fd, ev::READ);

    // Setup watcher for SIGINT
    signal_watcher.set<Server, &Server::sig_int>(this);
    signal_watcher.start(SIGINT);
  }

  ev::default_loop                          loop;
  ev::io                                    listener;
  ev::sig                                   signal_watcher;
  std::unordered_map<ev::timer*, Callback*> timers;
  std::unordered_set<ClientStream*>         streams_;
  int                                       port;
};

//---------------------------------------------------------
} // end of namespace evented
} // end of namespace jg
//---------------------------------------------------------

#undef CHECK

#endif
