#include <iostream>
#include "RPN.h"
#include "Scanner.h"


int main(int argc, char **argv) {
    std::string input;
    std::getline(std::cin, input);

    auto expression = Scanner::getExpression(input);
    rpn::convertToRPN(expression);
    Number result = rpn::eval(expression);
    std::cout << result.getValue() << std::endl;

    #if 0
    #endif
    return 0;
}
