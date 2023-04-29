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

    std::unique_ptr<ExprPart> ptr{new LPar{}};
    Expression expr1;
    expr1.push_back(std::move(ptr));


    return 0;
}
