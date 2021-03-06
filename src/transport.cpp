#include "broked/transport.h"

namespace bd
{

namespace transport
{

void send_multipart(zmq::socket_t& socket, const std::vector<std::string>& strs)
{
  for (std::vector<std::string>::size_type i = 0; i < strs.size() - 1; ++i)
  {
    zmq::message_t msg{strs[i].size()};
    memcpy(msg.data(), strs[i].data(), strs[i].size());
    socket.send(msg, ZMQ_SNDMORE);
  }

  zmq::message_t msg{strs.back().size()};
  memcpy(msg.data(), strs.back().data(), strs.back().size());
  socket.send(msg);
}

std::vector<std::string> recv_multipart(zmq::socket_t& socket)
{
  std::vector<std::string> result;

  int is_more = 0;
  do
  {
    zmq::message_t msg;
    socket.recv(&msg);
    result.emplace_back(static_cast<char *>(msg.data()), msg.size());

    size_t more_size = sizeof(is_more);
    socket.getsockopt(ZMQ_RCVMORE, &is_more, &more_size);
  } while (is_more);

  return result;
}

}

}
