#include <gtest/gtest.h>
// LOCAL INCLUDES
#include "theLibForTest/liba_1.h"

TEST(LibA, Add)
{
  hearc::liba::LibA_1 la1;	
  EXPECT_EQ(la1.Add(0, 0), 0);
  EXPECT_EQ(la1.Add(1, -1), 0);
  EXPECT_EQ(la1.Add(99, 99), 198);
  EXPECT_EQ(la1.Add(1, 2), 3);
}
