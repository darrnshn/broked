#pragma once

#include "socket.hpp"

namespace bd
{

socket make_socket(zmq::socket_t& socket)
{
  return {socket};
}

}
