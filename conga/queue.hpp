#pragma once

#include "message.hpp"
#include "transport.hpp"

#include <zmq.hpp>

#define CONGA_IDENTITY_LEN 5

namespace conga
{

namespace detail
{
  std::string generate_identity()
  {
    static const char table[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string s;
    for (int i = 0; i < CONGA_IDENTITY_LEN; i++)
      s += table[rand() % (sizeof(table) - 1)];
    return s;
  }
}

class queue
{
public:
  queue(std::string address, std::string name = "", std::string identity = "")
    : m_ctx{1}
    , m_msg_socket{m_ctx, ZMQ_DEALER}
    , m_address(std::move(address))
    , m_name(std::move(name))
    , m_identity(identity != "" ? identity : detail::generate_identity())
  {
    m_msg_socket.setsockopt(ZMQ_IDENTITY, m_identity.data(), m_identity.size());
    m_msg_socket.connect(m_address);
  }

  const std::string& address() const { return m_address; }
  const std::string& name() const { return m_name; }

  void push(std::string event, std::string data = "")
  {
    transport::send_message(m_msg_socket, std::move(event), std::move(data));
  }

private:
  zmq::context_t m_ctx;
  zmq::socket_t m_msg_socket;
  std::string m_address;
  std::string m_name;
  std::string m_identity;
};

}
