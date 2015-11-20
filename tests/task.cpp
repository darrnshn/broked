#include "gtest/gtest.h"
#include "broked/task.h"

TEST(Task, HasCorrectConstructor)
{
  bd::task t{"connect", "hello world"};
  EXPECT_EQ("connect", t.name);
  EXPECT_EQ("hello world", t.data);
}

TEST(Task, IsEqualToSameTask)
{
  bd::task t1{"connect", "hello world"};
  bd::task t2{"connect", "hello world"};
  EXPECT_EQ(t1, t2);
}

TEST(Task, IsNotEqualToDifferentTask)
{
  bd::task t1{"connect", "hello world"};
  bd::task t2{"connect", "hello world!"};

  EXPECT_NE(t1, t2);
}
