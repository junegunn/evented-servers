#include <cstring>
#include <arpa/inet.h>
#include "addr_info.h"
#include "util.h"
#include "exception.h"

using namespace std;
using namespace jg::evented;

AddrInfo::AddrInfo(int port) {
  // getaddrinfo
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family   = AF_UNSPEC;   // caller will accept any protocol family
  hints.ai_socktype = SOCK_STREAM; // TCP
  hints.ai_flags    = AI_PASSIVE;  // socket address structure for bind(2)

  if (getaddrinfo(NULL, util::to_string(port).c_str(), &hints, &info_) != 0) {
    throw Exception("getaddrinfo");
  }

  // print(cout);
}

AddrInfo::~AddrInfo() {
  freeaddrinfo(info_);
}

void AddrInfo::print(ostream& out) {
  struct addrinfo* ptr;
  for (ptr = info_; ptr != NULL; ptr = ptr->ai_next) {
    bool  ipv4 = ptr->ai_family == AF_INET;
    void* addr;
    char  ipstr[INET6_ADDRSTRLEN];

    if (ipv4) {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *) ptr->ai_addr;
      addr = &(ipv4->sin_addr);
    } else {
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
      addr = &(ipv6->sin6_addr);
    }

    // convert the IP to a string and print it:
    inet_ntop(ptr->ai_family, addr, ipstr, sizeof ipstr);
    out << (ipv4 ? "IPv4" : "IPv6")
        << ": "
        << ipstr
        << endl;
  }
}
