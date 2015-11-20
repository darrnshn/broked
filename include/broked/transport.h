#ifndef BROKED_TRANSPORT_H
#define BROKED_TRANSPORT_H

#include <zmq.hpp>

#include <vector>
#include <string>

namespace bd
{

namespace transport
{

void send_multipart(zmq::socket_t& socket, const std::vector<std::string>& strs);

std::vector<std::string> recv_multipart(zmq::socket_t& socket);

}

}

#endif
