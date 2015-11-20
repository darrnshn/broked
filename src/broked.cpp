#include "broked.hpp"

#define BROKED_IDENTITY_LEN 5

#include <zmq.hpp>

#include "socket.hpp"
#include "socket_internal.hpp"

namespace bd
{

namespace
{

/*! Generates a random socket identity.
 *
 *  \returns A random alphanumeric string of length BROKED_IDENTITY_LEN.
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

void connect(const std::string& address, const subscriber& sub, std::string identity)
{
  identity = identity != "" ? identity : detail::generate_identity();

  // Connect to the server
  zmq::context_t ctx{1};
  zmq::socket_t socket{ctx, ZMQ_DEALER};
  socket.setsockopt(ZMQ_IDENTITY, identity.data(), identity.size());
  socket.connect(address);

  // Wrap the zmq socket to be passed to the callbacks
  auto socket_wrapper = make_socket(socket);

  client::send_hello(socket, sub.subscribed_events());
  sub.trigger_connect(socket_wrapper);

  // Dispatch callbacks based on messages until the socket is closed.
  while (socket_wrapper.is_connected())
  {
    auto msg = transport::recv_message(socket);
    sub.trigger_callback(socket_wrapper, std::move(msg));
  }

  client::send_bye(socket);
  sub.trigger_disconnect();
}

}
