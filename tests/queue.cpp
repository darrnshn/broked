#include "gtest/gtest.h"
#include "queue.hpp"

TEST(Queue, CanInitializeWithAddress)
{
  conga::queue<int> q{"localhost:5000"};
  EXPECT_EQ("localhost::5000", q.address());
}

TEST(Queue, HasDefaultName)
{
  conga::queue<int> q{"localhost:5000"};
  EXPECT_EQ("", q.name());
}

TEST(Queue, CanInitializeWithName)
{
  conga::queue<int> q{"localhost:5000", "foo"};
  EXPECT_EQ("foo", q.name());
}

TEST(Queue, CanPushMessage)
{
  // Set up mock server
  nanomsg stuff

  conga::queue<int> q{"localhost:5000"};
  q.push(0, "foo");

  EXPECT_EQ("foo", msg.data());
}

TEST(Queue, CanPushAndReceiveMessage)
{
  conga::queue<int> q;
  q.push(0, "foo");
  q.on(0, [&q](conga::message&& msg) { q.close(); });
}

TEST(Queue, CanAttachSingleHandler)
{

}
