#include <iostream>
#include "Expression.h"

int main(int argc, char **argv) {
    std::string input;
    std::getline(std::cin, input);
    auto expression = Scanner::getExpression(input);
    Scanner::convertToRPN(expression);
    for (auto& ptr: expression) {
        std::cout << *ptr << " ";
    }
    std::cout << std::endl;

    Number result = eval(expression);
    std::cout << result.getValue() << std::endl;

    return 0;
}
