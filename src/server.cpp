#include "broked/server.hpp"

#include <zmq.hpp>

namespace bd
{

namespace
{

}

server::server()
{
}

server::listen(const std::string& address, int port)
{
  zmq::context_t ctx{1};
  zmq::socket_t socket{ctx, ZMQ_ROUTER};

  while (true)
  {
    auto msg = socket.recv_multipart(socket);
  }
}

void handle_message(const transport::message& m)
{
  if (m.type() == transport::message_type::hello)
  {
    // Register new client
    m_clients[m.address()] 
  }
}

}
