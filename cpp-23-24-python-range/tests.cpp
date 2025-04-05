#include "cpp-23-24-python-range.hpp"
#include <random>
#include <gtest/gtest.h>

std::vector<int> FromArray(const std::pair<int, const int*>& pair) {
  std::vector<int> res(pair.first, 0);
  for (int i = 0; i < pair.first; ++i) {
    res[i] = pair.second[i];
  }
  return res;
}

TEST(CORRECTNESS, Small) {
  {
    std::vector<int> expected{2, 3, 4};
    auto stud_ans = Range(2, 5, 1);
    EXPECT_EQ(expected, FromArray(stud_ans));
    delete[] stud_ans.second;
  }
  {
    std::vector<int> expected{1, 3};
    auto stud_ans = Range(1, 5, 2);
    EXPECT_EQ(expected, FromArray(stud_ans));
    delete[] stud_ans.second;
  }
  {
    std::vector<int> expected{-9, -4, 1, 6};
    auto stud_ans = Range(-9, 10, 5);
    EXPECT_EQ(expected, FromArray(stud_ans));
    delete[] stud_ans.second;
  }
}

TEST(CORRECTNESS, Reverse) {
  {
    std::vector<int> expected{5, 4, 3};
    auto stud_ans = Range(5, 2, -1);
    EXPECT_EQ(expected, FromArray(stud_ans));
    delete[] stud_ans.second;
  }
  {
    std::vector<int> expected{5, 3};
    auto stud_ans = Range(5, 1, -2);
    EXPECT_EQ(expected, FromArray(stud_ans));
    delete[] stud_ans.second;
  }
  {
    std::vector<int> expected{7};
    auto stud_ans = Range(7, 6, -1);
    EXPECT_EQ(expected, FromArray(stud_ans));
    delete[] stud_ans.second;
  }
}

TEST(Empty, Simple) {
  {
    auto stud_ans = Range(0, 0, 2);
    EXPECT_EQ(0, stud_ans.first);
    delete[] stud_ans.second;
  }
  {
    auto stud_ans = Range(2, 2, 1);
    EXPECT_EQ(0, stud_ans.first);
    delete[] stud_ans.second;
  }
  {
    auto stud_ans = Range(10, 10, -2);
    EXPECT_EQ(0, stud_ans.first);
    delete[] stud_ans.second;
  }
}


TEST(Empty, Inf) {
  {
    auto stud_ans = Range(3, 7, -1);
    EXPECT_EQ(0, stud_ans.first);
    delete[] stud_ans.second;
  }
  {
    auto stud_ans = Range(3, 4, 0);
    EXPECT_EQ(0, stud_ans.first);
    delete[] stud_ans.second;
  }
  {
    auto stud_ans = Range(5, -5, 2);
    EXPECT_EQ(0, stud_ans.first);
    delete[] stud_ans.second;
  }
  {
    auto stud_ans = Range(3, -7, 0);
    EXPECT_EQ(0, stud_ans.first);
    delete[] stud_ans.second;
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
