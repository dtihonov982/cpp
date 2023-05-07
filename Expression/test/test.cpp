#include <iostream>
#include <sstream>
#include <fstream>
#include <gtest/gtest.h>
#include "Expression.h"
#include "RPN.h"
#include "Scanner.h"

std::string getStr(std::string file) {
    std::ifstream f{file};
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

TEST(Scanner, input200k) {
    std::string input = getStr("input200k.txt");
    auto expression = Scanner::getExpression(input);
    rpn::convertToRPN(expression);
    Number result = rpn::eval(expression);
    EXPECT_EQ(result.getValue(), 199);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
