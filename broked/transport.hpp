#pragma once

#include "message.hpp"

#include <zmq.hpp>

namespace bd
{

namespace transport
{

struct message_type
{
  static const std::string hello;
  static const std::string message;
  static const std::string bye;
  static const std::string heartbeat;
};

const std::string message_type::hello = "1";
const std::string message_type::message = "2";
const std::string message_type::bye = "3";
const std::string message_type::heartbeat = "4";

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

void send_message(zmq::socket_t& socket, const message& msg)
{
  send_multipart(socket, { "", message_type::message, msg.event(), msg.data() });
}

void send_message(zmq::socket_t& socket, std::string&& event, std::string&& data)
{
  send_multipart(socket, { "", message_type::message, std::move(event), std::move(data) });
}

bd::message recv_message(zmq::socket_t& socket)
{
  auto result = recv_multipart(socket);
  // TODO: handle bad packets
  return { result[2], result[3] };
}

}

}
