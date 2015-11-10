#include "gtest/gtest.h"
#include "message.hpp"

TEST(Message, HasDefaultValues)
{
  bd::message msg;
  EXPECT_EQ("", msg.event());
  EXPECT_EQ("", msg.data());
  EXPECT_EQ(0, msg.size());
}

TEST(Message, CanInitializeWithEvent)
{
  bd::message msg{"connect"};
  EXPECT_EQ("connect", msg.event());
  EXPECT_EQ("", msg.data());
  EXPECT_EQ(0, msg.size());
}

TEST(Message, CanInitializeWithData)
{
  bd::message msg{"connect", "hello world"};
  EXPECT_EQ("connect", msg.event());
  EXPECT_EQ("hello world", msg.data());
  EXPECT_EQ(11, msg.size());
}

TEST(Message, IsEqualToItself)
{
  bd::message msg{"connect", "hello world"};
  EXPECT_EQ(msg, msg);
}

TEST(Message, IsNotEqualToDifferentMessage)
{
  bd::message msg1{"connect", "hello world"};
  bd::message msg2{"connect", "hello world!"};

  EXPECT_NE(msg1, msg2);
}
