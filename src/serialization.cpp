#include "broked/serialization.h"

#include <sstream>

namespace bd {

namespace {

std::vector<std::string> split(const std::string& s, char delim) {
  // from http://stackoverflow.com/questions/236129/split-a-string-in-c
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

std::string join(const std::vector<std::string>& v, char delim) {
  // from http://stackoverflow.com/questions/1430757/c-vector-to-string
  std::stringstream ss;
  for (size_t i = 0; i < v.size(); ++i) {
    if (i)
      ss << delim;
    ss << v[i];
  }
  return ss.str();
}

}

std::string socket_send_archive::to_bytes(const std::string& v) {
  return v;
}

std::string socket_send_archive::to_bytes(const std::vector<std::string>& v) {
  return join(v, '\0');
}

void socket_send_archive::send(const std::string& s, flag f) {
  zmq::message_t msg{s.size()};
  memcpy(msg.data(), s.data(), s.size());

  switch (f) {
  case flag::send_more:
    socket.send(msg, ZMQ_SNDMORE);
    break;
  case flag::send_last:
    socket.send(msg);
    break;
  }
}

void socket_recv_archive::recv(std::string& s) {
  zmq::message_t msg;
  socket.recv(&msg);
  s = std::string{static_cast<char *>(msg.data()), msg.size()};
}

void socket_recv_archive::recv(std::vector<std::string>& s) {
  zmq::message_t msg;
  socket.recv(&msg);
  s = split({ static_cast<char *>(msg.data()), msg.size() }, '\0');
}

bool socket_recv_archive::has_more() const {
  int is_more = 0;
  size_t more_size = sizeof(is_more);
  socket.getsockopt(ZMQ_RCVMORE, &is_more, &more_size);
  return is_more;
}

}
