#pragma once

#include "message.hpp"
#include "transport.hpp"

#include <zmq.hpp>

namespace bd
{

/*! \p socket is the main interface between a client and a broked server.
 *  It allows a client to send events to the server, which are then handled by
 *  other clients.
 */
class socket
{
public:
  /*! Sends an event to any client that can handle it. This may include the
   *  sender.
   *
   *  It is guaranteed that at most one client will handle the event.
   *
   *  \param event The event type.
   *  \param data The message.
   */
  void send_any(std::string event, std::string data = "")
  {
    transport::send_message(m_socket, std::move(event), std::move(data));
  }

  void send_all(std::string event, std::string data = "");

  /*! Close the socket.
   *
   *  When this is called, new events will no longer be handled by the client.
   *  Once all event handlers have finished, the associated \pp connect call
   *  will unblock.
   */
  void close()
  {
    // TODO: send disconnect message
    m_is_connected = false;
  }

  bool is_connected() const { return m_is_connected; }

private:
  socket(zmq::socket_t& socket)
    : m_socket(socket)
    , m_is_connected(true)
  {
  }

  zmq::socket_t& m_socket;
  bool m_is_connected;

  friend socket make_socket(zmq::socket_t& socket);
};

}
