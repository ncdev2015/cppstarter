#include <gtest/gtest.h>

// Simple function to test
int sum(int a, int b) {
    return a + b;
}

// Passing test
TEST(SumTests, PositiveNumbers) {
    EXPECT_EQ(sum(2, 3), 5);
}

// Passing test
TEST(SumTests, NegativeNumbers) {
    EXPECT_EQ(sum(-2, -3), -5);
}

// Failing test
TEST(SumTests, FailingTest) {
    EXPECT_EQ(sum(2, 2), 5);  // This will fail because 2 + 2 != 5
}

// Failing test with custom message
TEST(SumTests, FailingTestWithMessage) {
    int result = sum(10, -3);
    EXPECT_EQ(result, 8) << "Error: Expected 8 but got " << result;
}
