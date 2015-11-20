#ifndef BROKED_SERIALIZATION_H
#define BROKED_SERIALIZATION_H

#include <zmq.hpp>

#include <string>

namespace bd {

class socket_send_archive {
public:
  socket_send_archive(zmq::socket_t& socket)
    : socket(socket) { }

  template <class T>
  void operator()(T&& arg) {
    send(to_bytes(std::forward<T>(arg)), flag::send_last);
  }

  template <class T, class...Args>
  void operator()(T&& arg, Args&&...args) {
    send(to_bytes(std::forward<T>(arg)), flag::send_more);
    (*this)(std::forward<Args>(args)...);
  }

private:
  enum class flag
  {
    send_last,
    send_more
  };

  std::string to_bytes(const std::string& v);
  std::string to_bytes(const std::vector<std::string>& v);

  void send(const std::string& s, flag f);

  zmq::socket_t& socket;
};

class socket_recv_archive {
public:
  socket_recv_archive(zmq::socket_t& socket)
    : socket(socket) { }

  template <class T>
  void operator()(T& arg) {
    recv(arg);
  }

  template <class T, class...Args>
  void operator()(T& arg, Args&...args) {
    recv(arg);
    if (has_more())
      (*this)(args...);
  }

private:
  void recv(std::string& v);
  void recv(std::vector<std::string>& v);

  bool has_more() const;

  zmq::socket_t& socket;
};

}

#endif // BROKED_SERIALIZATION_H
