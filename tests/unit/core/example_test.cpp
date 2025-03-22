#include <gtest/gtest.h>

// Simple test to verify the test framework is working
TEST(ExampleTest, BasicTest) {
    EXPECT_EQ(2 + 2, 4);
}

// Another test to demonstrate different assertions
TEST(ExampleTest, StringTest) {
    std::string str = "Hello, World!";
    EXPECT_FALSE(str.empty());
    EXPECT_EQ(str.length(), 13);
}