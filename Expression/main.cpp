#include <iostream>
#include <gtest/gtest.h>
#include "Expression.h"

TEST(Scanner, test1) {
    std::string input = "2 + 2 - (3 - 7)";
    auto expr = Scanner::getExpression(input);
    EXPECT_EQ(expr.size(), 9);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
