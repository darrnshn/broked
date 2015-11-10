#include "gtest/gtest.h"
#include "broked.hpp"

#include "socket.hpp"
#include <zmq.hpp>
#include <future>

class BrokedMockServerTest : public ::testing::Test
{
public:
  BrokedMockServerTest()
    : ctx{1}
    , server{ctx, ZMQ_ROUTER}
  {
  }

protected:
  virtual void SetUp()
  {
    server.bind("tcp://*:5555");
  }

  zmq::context_t ctx;
  zmq::socket_t server;
};

TEST_F(BrokedMockServerTest, ConnectUnblocksWhenSocketIsClosed)
{
  bool called = false;

  bd::subscriber sub;
  sub.on_connect([&called](bd::socket& s) { s.close(); called = true; });
  bd::connect("tcp://localhost:5555", sub);

  EXPECT_TRUE(called);
}

TEST_F(BrokedMockServerTest, SendsHelloToServerWhenConnected)
{
  bd::subscriber sub;
  sub.on_connect([](bd::socket& s) { s.close(); });
  bd::connect("tcp://localhost:5555", sub, "identity");

  auto result = bd::transport::recv_multipart(server);

  ASSERT_EQ(3, result.size());
  EXPECT_EQ("identity", result[0]);
  EXPECT_EQ("", result[1]);
  EXPECT_EQ(bd::transport::message_type::hello, result[2]);
}

TEST_F(BrokedMockServerTest, SendsByeToServerWhenDisconnected)
{
  bd::subscriber sub;
  sub.on_connect([](bd::socket& s) { s.close(); });
  bd::connect("tcp://localhost:5555", sub, "identity");

  bd::transport::recv_multipart(server); // Ignore hello message
  auto result = bd::transport::recv_multipart(server);

  ASSERT_EQ(3, result.size());
  EXPECT_EQ("identity", result[0]);
  EXPECT_EQ("", result[1]);
  EXPECT_EQ(bd::transport::message_type::bye, result[2]);
}

TEST_F(BrokedMockServerTest, CanSendMessageToServerAfterConnect)
{
  bd::subscriber sub;
  sub.on_connect([](bd::socket& s) { s.send_any("ping", "hi"); s.close(); });
  bd::connect("tcp://localhost:5555", sub, "identity");

  bd::transport::recv_multipart(server); // Ignore hello message
  auto result = bd::transport::recv_multipart(server);

  ASSERT_EQ(5, result.size());
  EXPECT_EQ("identity", result[0]);
  EXPECT_EQ("", result[1]);
  EXPECT_EQ(bd::transport::message_type::message, result[2]);
  EXPECT_EQ("ping", result[3]);
  EXPECT_EQ("hi", result[4]);
}

TEST_F(BrokedMockServerTest, CanReceiveMessageFromServer)
{
  bd::subscriber sub;
  sub.on("ping", [](bd::socket& s, bd::message&& m)
  {
    EXPECT_EQ("ping", m.event());
    EXPECT_EQ("hi", m.data());
    s.close();
  });

  auto future = std::async(std::launch::async,
      [&]() { bd::connect("tcp://localhost:5555", sub, "identity"); } );

  bd::transport::recv_multipart(server); // receive HELLO
  bd::transport::send_multipart(server,
      { "identity", "", bd::transport::message_type::message, "ping", "hi" });

  future.wait();
}

TEST_F(BrokedMockServerTest, CallsCorrectCallback)
{
  bool called_ping = false, called_pong = false;

  bd::subscriber sub;
  sub.on("ping", [&](bd::socket& s, bd::message&& m)
  {
    called_ping = true;
    s.close();
  });

  sub.on("pong", [&](bd::socket& s, bd::message&& m)
  {
    called_pong = true;
    s.close();
  });

  auto future = std::async(std::launch::async,
      [&]() { bd::connect("tcp://localhost:5555", sub, "identity"); } );

  bd::transport::recv_multipart(server); // receive HELLO
  bd::transport::send_multipart(server,
      { "identity", "", bd::transport::message_type::message, "ping", "hi" });
  bd::transport::recv_multipart(server); // receive BYE

  EXPECT_TRUE(called_ping);
  EXPECT_FALSE(called_pong);
}

TEST_F(BrokedMockServerTest, CanConnectMultipleTimes)
{
  bd::subscriber sub;
  sub.on_connect([](bd::socket& s) { s.close(); });
  bd::connect("tcp://localhost:5555", sub);
  bd::connect("tcp://localhost:5555", sub);
  bd::connect("tcp://localhost:5555", sub);
}
