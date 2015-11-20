#ifndef BROKED_CHANNEL_H
#define BROKED_CHANNEL_H

#include "task.h"

#include <zmq.hpp>

namespace bd {

class channel {
public:
  void send(task t);

  void close();
  bool is_connected() const { return m_is_connected; }

private:
  channel(zmq::socket_t& socket)
    : m_socket(socket)
    , m_is_connected(true)
  {
  }

  zmq::socket_t& m_socket;
  bool m_is_connected;

  friend socket make_socket(zmq::socket_t& socket);
};

}

#endif // BROKED_CHANNEL_H
