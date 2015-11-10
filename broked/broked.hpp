#pragma once

#define BROKED_VERSION 0

#define BROKED_MAJOR_VERSION    (BROKED_VERSION / 100000)
#define BROKED_MINOR_VERSION    (BROKED_VERSION / 100 % 1000)
#define BROKED_SUBMINOR_VERSION (BROKED_VERSION % 100)

#include "socket.hpp"
#include "socket_internal.hpp"
#include "subscriber.hpp"

#include <zmq.hpp>

#define BROKED_IDENTITY_LEN 5

namespace bd
{

namespace detail
{

/*! Generates a random socket identity.
 *
 *  \return A random alphanumeric string of length BROKED_IDENTITY_LEN.
 */
std::string generate_identity()
{
  static const char table[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

  char s[BROKED_IDENTITY_LEN];
  std::generate(std::begin(s), std::end(s),
    []() { return table[rand() % (sizeof(table) - 1)]; });
  return s;
}

}

/*! Connects to a broked server.
 *
 *  \param address The address of the broked server.
 *  \param sub A subscriber to handle events received from the server.
 */
void connect(const std::string& address, const subscriber& sub, std::string identity = "")
{
  identity = identity != "" ? identity : detail::generate_identity();

  zmq::context_t ctx{1};
  zmq::socket_t socket{ctx, ZMQ_DEALER};
  socket.setsockopt(ZMQ_IDENTITY, identity.data(), identity.size());
  socket.connect(address);

  // Wrap the zmq socket to be passed to the callbacks
  auto socket_wrapper = make_socket(socket);

  // Send HELLO message to the server
  transport::send_multipart(socket, { "", transport::message_type::hello });

  sub.trigger_connect(socket_wrapper);

  // Wait for messages
  while (socket_wrapper.is_connected())
  {
    // Get the next message and dispatch to the correct callback
    auto msg = transport::recv_multipart(socket);

    // TODO: drop bad packets
    if (msg[1] == transport::message_type::message)
      sub.trigger_callback(socket_wrapper, { msg[2], msg[3] });
  }

  // Send BYE message to the server
  transport::send_multipart(socket, { "", transport::message_type::bye });
}

}
