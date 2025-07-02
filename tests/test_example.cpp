#include <gtest/gtest.h>
#include <string>
#include <vector>
// If you have classes in your project, include them here
// #include "your_class.hpp"

// Basic test example
TEST(BasicTest, SimpleAssertions) {
    EXPECT_EQ(2 + 2, 4);
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
    EXPECT_STREQ("hello", "hello");
}

// String test example
TEST(StringTest, StringOperations) {
    std::string str = "Hello, World!";
    
    EXPECT_EQ(str.length(), 13);
    EXPECT_EQ(str.substr(0, 5), "Hello");
    EXPECT_NE(str.find("World"), std::string::npos);
}

// Vector test example
TEST(VectorTest, VectorOperations) {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec.back(), 5);
    
    vec.push_back(6);
    EXPECT_EQ(vec.size(), 6);
    EXPECT_EQ(vec.back(), 6);
}

// Expected exception test example
TEST(ExceptionTest, ExpectedException) {
    EXPECT_THROW({
        std::vector<int> vec(5);
        vec.at(10);  // Should throw std::out_of_range
    }, std::out_of_range);
}

// Test fixture example (for setup/teardown)
class MathTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Code that runs before each test
        a = 10;
        b = 5;
    }
    
    void TearDown() override {
        // Code that runs after each test
        // In this case we don't need to clean up anything
    }
    
    int a, b;
};

TEST_F(MathTest, Addition) {
    EXPECT_EQ(a + b, 15);
}

TEST_F(MathTest, Subtraction) {
    EXPECT_EQ(a - b, 5);
}

TEST_F(MathTest, Multiplication) {
    EXPECT_EQ(a * b, 50);
}

TEST_F(MathTest, Division) {
    EXPECT_EQ(a / b, 2);
    EXPECT_THROW(a / 0, std::exception);  // This might not work with integer division
}

// Parametrized test example
class ParametrizedTest : public ::testing::TestWithParam<int> {
public:
    bool isPrime(int n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }
};

TEST_P(ParametrizedTest, PrimeTest) {
    int value = GetParam();
    EXPECT_TRUE(isPrime(value));
}

INSTANTIATE_TEST_SUITE_P(
    PrimeNumbers,
    ParametrizedTest,
    ::testing::Values(2, 3, 5, 7, 11, 13, 17, 19, 23)
);

// Test that should fail (commented out so it doesn't fail the build)
/*
TEST(FailingTest, ThisWillFail) {
    EXPECT_EQ(1, 2) << "One is not equal to two!";
}
*/