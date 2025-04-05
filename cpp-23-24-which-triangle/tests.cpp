#include "cpp-23-24-which-triangle.hpp"
#include <gtest/gtest.h>

void CheckCase(int res, int a, int b, int c) {
  EXPECT_EQ(WhichTriangle(a, b, c), res);
  EXPECT_EQ(WhichTriangle(a, c, b), res);
  EXPECT_EQ(WhichTriangle(b, a, c), res);
  EXPECT_EQ(WhichTriangle(b, c, a), res);
  EXPECT_EQ(WhichTriangle(c, a, b), res);
  EXPECT_EQ(WhichTriangle(c, b, a), res);
}

TEST(Correctness, Small) {
  {
    CheckCase(0, 10, 2, 4);
  }
  {
    CheckCase(1, 3, 2, 5);
  }
  {
    CheckCase(2, 5, 12, 11);
  }
  {
    CheckCase(3, 5, 12, 13);
  }
  {
    CheckCase(4, 5, 12, 16);
  }
}

TEST(Correctness, Precision) {
  {
    CheckCase(0, 5e8, 4e8, 1e9);
  }
  {
    CheckCase(1, 5e8, 1e9, 5e8);
  }
  {
    CheckCase(2, 8e8, 7e8, 1e9);
  }
  {
    CheckCase(3, 3e8, 5e8, 4e8);
  }
  {
    CheckCase(4, 1e9, 6e8, 7e8);
  }
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
