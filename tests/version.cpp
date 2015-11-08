#include "gtest/gtest.h"
#include "conga.hpp"

TEST(Version, IsUpToDate)
{
  EXPECT_EQ(0, CONGA_MAJOR_VERSION);
  EXPECT_EQ(0, CONGA_MINOR_VERSION);
  EXPECT_EQ(0, CONGA_SUBMINOR_VERSION);
}
