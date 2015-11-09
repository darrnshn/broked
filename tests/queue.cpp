#include "gtest/gtest.h"
#include "queue.hpp"
#include "transport.hpp"

class QueueMockServerTest : public ::testing::Test
{
public:
  QueueMockServerTest()
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


TEST(Queue, CanInitializeWithAddress)
{
  conga::queue q{"tcp://localhost:5555"};
  EXPECT_EQ("tcp://localhost:5555", q.address());
}

TEST(Queue, HasDefaultName)
{
  conga::queue q{"tcp://localhost:5555"};
  EXPECT_EQ("", q.name());
}

TEST(Queue, CanInitializeWithName)
{
  conga::queue q{"tcp://localhost:5555", "foo"};
  EXPECT_EQ("tcp://localhost:5555", q.address());
  EXPECT_EQ("foo", q.name());
}

TEST_F(QueueMockServerTest, CanPushAMessage)
{
  conga::queue q{"tcp://localhost:5555", "", "identity"};
  q.push("event", "data");

  auto result = conga::transport::detail::recv_multipart(server);

  ASSERT_EQ(3, result.size());
  EXPECT_EQ("identity", result[0]);
  EXPECT_EQ("event", result[1]);
  EXPECT_EQ("data", result[2]);
}

TEST_F(QueueMockServerTest, CanPushMultipleMessages)
{
  conga::queue q{"tcp://localhost:5555", "", "identity"};
  q.push("event", "1");
  q.push("event", "2");

  auto result1 = conga::transport::detail::recv_multipart(server);
  auto result2 = conga::transport::detail::recv_multipart(server);

  EXPECT_EQ("identity", result1[0]);
  EXPECT_EQ("event", result1[1]);
  EXPECT_EQ("1", result1[2]);
  EXPECT_EQ("identity", result2[0]);
  EXPECT_EQ("event", result2[1]);
  EXPECT_EQ("2", result2[2]);
}
