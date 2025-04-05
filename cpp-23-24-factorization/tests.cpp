#include "cpp-23-24-factorization.hpp"
#include <gtest/gtest.h>

TEST(CORRECTNESS, PRIME) {
    auto factors = Factorize(131'071);

    std::vector<int> ans = {131'071};

    EXPECT_EQ(factors, ans);
}

TEST(CORRECTNESS, COMPOSITE) {
    auto factors = Factorize(1218);

    std::vector<int> ans = {2, 3, 7, 29};

    EXPECT_EQ(factors, ans);
}

TEST(CORRECTNESS, COMPOSITE_WITH_DUPLICATES) {
    auto factors = Factorize(792);

    std::vector<int> ans = {2, 2, 2, 3, 3, 11};

    EXPECT_EQ(factors, ans);
}

TEST(CORRECTNESS, SQUARE_NUM) {
    auto factors = Factorize(841);

    std::vector<int> ans = {29, 29};

    EXPECT_EQ(factors, ans);
}

TEST(CORRECTNESS, SMALL_GUY) {
    auto factors = Factorize(2);

    std::vector<int> ans = {2};

    EXPECT_EQ(factors, ans);
}

TEST(CORRECTNESS, BIG_GUY) {
    auto factors = Factorize(2'147'483'647);

    std::vector<int> ans = {2'147'483'647};

    EXPECT_EQ(factors, ans);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}