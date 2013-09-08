#ifndef __jg_addr_info__
#define __jg_addr_info__

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sstream>

//---------------------------------------------------------
namespace jg {
namespace evented {
//---------------------------------------------------------

// RAII helper
class AddrInfo {
public:
  AddrInfo(int port);
  ~AddrInfo();

  void print(std::ostream& out);
  inline struct addrinfo* const info() { return info_; }
private:
  struct addrinfo* info_;
};

//---------------------------------------------------------
} // end of namespace evented
} // end of namespace jg
//---------------------------------------------------------

#endif
