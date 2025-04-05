#include "cpp-23-24-pointwise-sum.hpp"
#include <random>
#include <gtest/gtest.h>

class VectorGenerator {
 public:
  explicit VectorGenerator(int max) :
    distrib_(std::uniform_int_distribution<>(-max, max)) {}
  std::vector<int> GenerateVector(int size) {
    std::vector<int> res(size, 0);
    for (int& elem : res) {
      elem = distrib_(gen_);
    }
    return res;
  }

 private:
  static std::mt19937 gen_;
  std::uniform_int_distribution<> distrib_;
};

std::mt19937 VectorGenerator::gen_ = std::mt19937(311);

std::vector<int> PointWiseSum(const std::vector<int>& a,
                              const std::vector<int>& b) {
  std::vector<int> res(a.size(), 0);
  for (int i = 0; i < a.size(); ++i) {
    res[i] = a[i] + b[i];
  }
  return res;
}

std::vector<int> FromArray(const int* arr, int n) {
  return {arr, arr + n};
}

TEST(CORRECTNESS, Small) {
  const int size = 10;
  const int max = 10;
  VectorGenerator gen(max);
  auto lhs = gen.GenerateVector(size);
  auto rhs = gen.GenerateVector(size);
  auto stud_res = PointwiseSum(lhs.data(), rhs.data(), size);
  EXPECT_EQ(PointWiseSum(lhs, rhs), FromArray(stud_res, size));
  delete[] stud_res;
}

TEST(CORRECTNESS, Medium) {
  const int size = 100;
  const int max = 1e9;
  VectorGenerator gen(max);
  auto lhs = gen.GenerateVector(size);
  auto rhs = gen.GenerateVector(size);
  auto stud_res = PointwiseSum(lhs.data(), rhs.data(), size);
  EXPECT_EQ(PointWiseSum(lhs, rhs), FromArray(stud_res, size));
  delete[] stud_res;
}

TEST(CORRECTNESS, Large) {
  const int size = 1e5;
  const int max = 1e9;
  VectorGenerator gen(max);
  auto lhs = gen.GenerateVector(size);
  auto rhs = gen.GenerateVector(size);
  auto stud_res = PointwiseSum(lhs.data(), rhs.data(), size);
  EXPECT_EQ(PointWiseSum(lhs, rhs), FromArray(stud_res, size));
  delete[] stud_res;
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
