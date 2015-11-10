#include "gtest/gtest.h"
#include "socket.hpp"
#include "socket_internal.hpp"

#include "transport.hpp"

class SocketMockServerTest : public ::testing::Test
{
public:
  SocketMockServerTest()
    : ctx{1}
    , server{ctx, ZMQ_ROUTER}
    , client{ctx, ZMQ_DEALER}
    , s(bd::make_socket(client))
  {
  }

protected:
  virtual void SetUp()
  {
    server.bind("tcp://*:5555");
    client.setsockopt(ZMQ_IDENTITY, "identity", 8);
    client.connect("tcp://localhost:5555");
  }

  zmq::context_t ctx;
  zmq::socket_t server;
  zmq::socket_t client;
  bd::socket s;
};

TEST_F(SocketMockServerTest, CanSendAny)
{
  s.send_any("event", "data");

  auto result = bd::transport::recv_multipart(server);

  ASSERT_EQ(5, result.size());
  EXPECT_EQ("identity", result[0]);
  EXPECT_EQ("", result[1]);
  EXPECT_EQ(bd::transport::message_type::message, result[2]);
  EXPECT_EQ("event", result[3]);
  EXPECT_EQ("data", result[4]);
}

TEST_F(SocketMockServerTest, CanSendAnyMultipleTimes)
{
  s.send_any("event", "1");
  s.send_any("event", "2");

  auto result1 = bd::transport::recv_multipart(server);
  auto result2 = bd::transport::recv_multipart(server);

  ASSERT_EQ(5, result1.size());
  EXPECT_EQ("identity", result1[0]);
  EXPECT_EQ("", result1[1]);
  EXPECT_EQ(bd::transport::message_type::message, result1[2]);
  EXPECT_EQ("event", result1[3]);
  EXPECT_EQ("1", result1[4]);

  ASSERT_EQ(5, result2.size());
  EXPECT_EQ("identity", result2[0]);
  EXPECT_EQ("", result2[1]);
  EXPECT_EQ(bd::transport::message_type::message, result1[2]);
  EXPECT_EQ("event", result2[3]);
  EXPECT_EQ("2", result2[4]);
}
