#include "RPN.h"

#include <memory>
#include <stack>
#include <utility>
#include <list>
#include <cassert>

void rpn::convertToRPN(Expression& expression) {
    Expression rpn;
    std::stack<std::unique_ptr<ExprPart>> stack;
    for (std::unique_ptr<ExprPart>& current: expression) {
        if (dynamic_cast<Number*>(current.get())) {
            rpn.push_back(std::move(current));
        }
        if (dynamic_cast<BinOperator*>(current.get())) {
            auto rawCurrent = dynamic_cast<IPrecedence*>(current.get());
            while (!stack.empty()) {
                std::unique_ptr<ExprPart>& top = stack.top();
                auto rawTop = dynamic_cast<IPrecedence*>(top.get());
                if (rawTop && rawTop->getPrecedence() >= rawCurrent->getPrecedence()) {
                    rpn.push_back(std::move(top));
                    stack.pop();
                }
                else {
                    break;
                }
            }
            stack.push(std::move(current));
        }
        if (dynamic_cast<UnaryOperator*>(current.get())) {
            stack.push(std::move(current));
        }
        if (dynamic_cast<Function*>(current.get())) {
            stack.push(std::move(current));
        }
        if (dynamic_cast<LPar*>(current.get())) {
            stack.push(std::move(current));
        }
        if (dynamic_cast<RPar*>(current.get())) {
            while (!stack.empty()) {
                auto& top = stack.top();
                if (!dynamic_cast<LPar*>(top.get()))  {
                    rpn.push_back(std::move(top));
                    stack.pop();
                }
                else {
                    stack.pop();
                    break;
                }
            }
            if (!stack.empty() && dynamic_cast<Function*>(stack.top().get())) {
                rpn.push_back(std::move(stack.top()));
                stack.pop();
            }
        }
    }
    while (!stack.empty()) {
        std::unique_ptr<ExprPart>& top = stack.top();
        rpn.push_back(std::move(top));
        stack.pop();
    }
    expression = std::move(rpn);
}

//TODO: rpn.size() == 1
Number rpn::eval(Expression& rpnExpr) {
    std::stack<ExprPart*> stack;
    //Using list here because I need storage pointers on his elements in stack and vector invalidates pointers.
    std::list<Number> tmpNumbers;
    for (auto& current: rpnExpr) {
        ExprPart* rawCurrent = current.get();
        if (dynamic_cast<Number*>(rawCurrent)) {
            stack.push(rawCurrent);
        }
        else if (auto oper = dynamic_cast<BinOperator*>(rawCurrent)) {
            assert(!stack.empty());

            Number* second = static_cast<Number*>(stack.top());
            stack.pop();
            Number* first = static_cast<Number*>(stack.top());
            stack.pop();
            
            tmpNumbers.emplace_back(oper->eval(first, second));
            stack.push(&tmpNumbers.back());
        }
        else if (auto oper = dynamic_cast<UnaryOperator*>(rawCurrent)) {
            assert(!stack.empty());
            Number* num = static_cast<Number*>(stack.top());
            stack.pop();
            tmpNumbers.push_back(oper->eval(num));
            stack.push(&tmpNumbers.back());
        }
        else if (auto func = dynamic_cast<Function*>(rawCurrent)) {
            std::vector<const Number*> argv;
            for(int i = func->argc(); i > 0; --i) {
                Number* top = static_cast<Number*>(stack.top());
                stack.pop();
                argv.push_back(top);
            }
            tmpNumbers.push_back(func->eval(argv));
            stack.push(&tmpNumbers.back());
        }
    }
    return tmpNumbers.empty() ? *static_cast<Number*>(stack.top()) : tmpNumbers.back();
}

