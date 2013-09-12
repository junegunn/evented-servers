//---------------------------------------------------------
namespace jg {
namespace evented {
//---------------------------------------------------------

template<class Server, class Stream>
class Handler
{
public:
  Handler() {}
  void on_open(Stream* stream) {}
  void on_close(Stream* stream) {}
  void on_data(Stream* stream, const char* data, int size) {}
};

//---------------------------------------------------------
} // end of namespace evented
} // end of namespace jg
//---------------------------------------------------------
