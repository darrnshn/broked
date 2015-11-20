#include "gtest/gtest.h"
#include "server.hpp"

#include "transport.hpp"

class ServerMockClientTest : public ::testing::Test
{
public:
  ServerMockClientTest()
    : ctx{1}
    , client{ctx, ZMQ_DEALER}
    , s(bd::make_socket(client))
  {
  }

protected:
  virtual void SetUp()
  {
    server.bind(5555);
    client.setsockopt(ZMQ_IDENTITY, "identity", 8);
    client.connect("tcp://localhost:5555");
  }

  zmq::context_t ctx;
  bd::server server;
  zmq::socket_t client1;
  zmq::socket_t client2;
};

TEST_F(ServerMockClientTest, CanRelayMessages)
{
  server.start();

  bd::client::send_hello(client, { "ping" });
  bd::client::send_message(client, "ping", "hi");

  auto msg = bd::client::recv_message(client);
  EXPECT_EQ("ping", msg.event());
  EXPECT_EQ("hi", msg.data());
}

TEST_F(ServerMockClientTest, RelaysMessageToClientThatCanHandleIt)
{
  server.start();

  bd::client::send_hello(client1, { "ping" });
  bd::client::send_message(client1, "ping", "hi");

  bd::client::send_hello(client2, { "pong" });

  auto msg = bd::client::recv_message(client2);
  EXPECT_EQ("pong", msg.event());
  EXPECT_EQ("hi", msg.data());
}
