#include "gtest/gtest.h"
#include "broked/broked.h"

TEST(Version, IsUpToDate)
{
  EXPECT_EQ(0, BROKED_MAJOR_VERSION);
  EXPECT_EQ(0, BROKED_MINOR_VERSION);
  EXPECT_EQ(0, BROKED_SUBMINOR_VERSION);
}
