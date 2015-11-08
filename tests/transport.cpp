#include "gtest/gtest.h"
#include <zmq.hpp>

TEST(Transport, CanUseZeroMQ)
{
  // Sanity check for ZeroMQ sockets
  zmq::context_t ctx{1};
  zmq::socket_t req{ctx, ZMQ_REQ}, rep{ctx, ZMQ_REP};
  rep.bind("tcp://*:5555");
  req.connect("tcp://localhost:5555");

  // Send message from req
  std::string data = "hello";
  zmq::message_t msg{data.size()};
  memcpy(msg.data(), data.data(), data.size());

  req.send(msg);

  // Receive message from rep
  zmq::message_t reply;
  rep.recv(&reply);
  std::string result{static_cast<char *>(reply.data()), reply.size()};

  EXPECT_EQ(data, result);
}
