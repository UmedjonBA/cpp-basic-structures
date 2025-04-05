#include "cpp-23-24-2d-spiral.hpp"
#include <gtest/gtest.h>

#include <fstream>
#include <string>
#include <vector>

std::vector<std::vector<int>> From2dArray(int** array, int n) {
  std::vector<std::vector<int>> res(n, std::vector<int>(n, 0));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      res[i][j] = array[i][j];
    }
  }
  return res;
}

std::istream& FromFile(std::istream& is, int n, std::vector<std::vector<int>>& res) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      is >> res[i][j];
    }
  }
  return is;
}

void Del2dArray(int** array, int n) {
  for (int i = 0; i < n; ++i) {
    delete[] array[i];
  }
  delete[] array;
}

TEST(CORRECTNESS, SMALL) {
  const int size = 5;
  std::string filename = "../small.ans";
  std::ifstream s(filename);
  if (s.is_open()) {
    std::vector<std::vector<int>> expected(size, std::vector<int>(size, 0));
    FromFile(s, size, expected);
    auto participant = MakeSpiral(size);
    EXPECT_EQ(expected, From2dArray(participant, size));
    Del2dArray(participant, size);
    s.close();
  } else {
    throw "File can't be open\n";
  }
}

TEST(CORRECTNESS, MEDIUM) {
  const int size = 10;
  std::string filename = "../medium.ans";
  std::ifstream s(filename);
  if (s.is_open()) {
    std::vector<std::vector<int>> expected(size, std::vector<int>(size, 0));
    FromFile(s, size, expected);
    auto participant = MakeSpiral(size);
    EXPECT_EQ(expected, From2dArray(participant, size));
    Del2dArray(participant, size);
    s.close();
  } else {
    throw "File can't be open\n";
  }
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}