#include "gtest/gtest.h"
#include "transport.hpp"

#include <zmq.hpp>
#include <vector>

class TransportReqRepTest : public ::testing::Test
{
public:
  TransportReqRepTest()
    : ctx{1}
    , req{ctx, ZMQ_REQ}
    , rep{ctx, ZMQ_REP}
  {
  }

protected:
  virtual void SetUp()
  {
    rep.bind("tcp://*:5555");
    req.connect("tcp://localhost:5555");
  }

  zmq::context_t ctx;
  zmq::socket_t req, rep;
};

TEST_F(TransportReqRepTest, CanUseZeroMQ)
{
  // Send message from req
  std::string data = "hello";
  zmq::message_t msg{data.size()};
  memcpy(msg.data(), data.data(), data.size());
  req.send(msg);

  // Receive message from rep
  zmq::message_t result;
  rep.recv(&result);
  std::string result_data{static_cast<char *>(result.data()), result.size()};

  EXPECT_EQ(data, result_data);
}

TEST_F(TransportReqRepTest, CanSendAndReceiveMessage)
{
  bd::message msg{"connect", "hello"};
  bd::transport::send_message(req, msg);

  auto result = bd::transport::recv_message(rep);

  EXPECT_EQ(msg, result);
}

TEST_F(TransportReqRepTest, CanEmplaceAndReceiveMessage)
{
  bd::transport::send_message(req, "connect", "hello");

  auto result = bd::transport::recv_message(rep);

  EXPECT_EQ("connect", result.event());
  EXPECT_EQ("hello", result.data());
}
