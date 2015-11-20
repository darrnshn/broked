#include "gtest/gtest.h"
#include "broked/result.h"

TEST(Result, HasCorrectConstructor)
{
  const bd::result r{"connect", "hello world"};
  EXPECT_EQ("connect", r.task);
  EXPECT_EQ("hello world", r.data);
}

TEST(Result, IsEqualToSameResult)
{
  const bd::result r1{"connect", "hello world"};
  const bd::result r2{"connect", "hello world"};
  EXPECT_EQ(r1, r2);
}

TEST(Result, IsNotEqualToDifferentResult)
{
  const bd::result r1{"connect", "hello world"};
  const bd::result r2{"connect", "hello world!"};

  EXPECT_NE(r1, r2);
}
