#include "gtest/gtest.h"
#include "message.hpp"

TEST(Message, HasDefaultValues)
{
  conga::message msg;
  EXPECT_EQ("", msg.event());
  EXPECT_EQ("", msg.data());
  EXPECT_EQ(0, msg.size());
}

TEST(Message, CanInitializeWithEvent)
{
  conga::message msg{"connect"};
  EXPECT_EQ("connect", msg.event());
  EXPECT_EQ("", msg.data());
  EXPECT_EQ(0, msg.size());
}

TEST(Message, CanInitializeWithData)
{
  conga::message msg{"connect", "hello world"};
  EXPECT_EQ("connect", msg.event());
  EXPECT_EQ("hello world", msg.data());
  EXPECT_EQ(11, msg.size());
}
