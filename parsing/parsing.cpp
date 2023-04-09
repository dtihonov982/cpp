#include <iostream>
#include <stack>
#include <queue>
#include <string>
#include <sstream>
#include <regex>
#include <map>
#include <exception>
#include <algorithm>
#include <cassert>
#include "Token.h"


//todo: rename ExpressionPart
class Word {
public:
    Word(WordType type_): type(type_) {}
    WordType getType() { return type; }
    virtual ~Word() {}
private:
    WordType type;
};

using ExprPart = Word;

class Number: public Word {
public:
    Number(const Token& token): Word(WordType::number) {
        value = std::stoi(token.str);
    }
    //todo: how to make this constructor without double base ctor calling
    Number(int n): Word(WordType::number), value(n) {}
    int getValue() const { return value; }
private:
    int value;
};

class Operator: public Word {
public:
    Operator(const Token& token_, int precedence_): Word(WordType::op), token(token_), precedence(precedence_) {}
    int getPrecedence() { return precedence; }

    Number eval(const Number& first, const Number& second) {
        int x = first.getValue();
        int y = second.getValue();
        int result;
        if (token.str == "+") {
            result = add(x, y);
        }
        if (token.str == "-") {
            result = sub(x, y);
        }
        if (token.str == "*") {
            result = mult(x, y);
        }
        if (token.str == "/") {
            result = div(x, y);
        }
        return Number{result};
    }


private:
    int precedence;
    Token token;

    static inline int add(int x, int y) { return x + y; }
    static inline int sub(int x, int y) { return x - y; }
    static inline int mult(int x, int y) { return x * y; }
    static inline int div(int x, int y) { return x / y; }
};

class LPar: public Word {
public:
    LPar(): Word(WordType::LPar) {}
};

class RPar: public Word {
public:
    RPar(): Word(WordType::RPar) {}
};

class WordFactory {
public:
    static Word* makeWord(const Token& token) {
        Word* result;
        switch (token.type) {
        case WordType::number:
            result = new Number{token};
            break;
        case WordType::op:
            result = new Operator{token, getOpPrecedence(token)};
            break;
        case WordType::LPar:
            result = new LPar{};
            break;
        case WordType::RPar:
            result = new RPar{};
            break;
        }
        return result;
    }

    
    static std::vector<Word*> convertToWords(const std::vector<Token>& input) {
        std::vector<Word*> result;
        for (const Token& token: input) {
            result.push_back(makeWord(token));
        }
        return result;
    }
};

class Processor {
public:
    static Number* eval(const std::vector<Word*>& rpnExpression) {
        if (rpnExpression.empty()) {
            return nullptr;
       }
       std::stack<Word*> Stack;
       for (auto exprPart: rpnExpression) {
            if (exprPart->getType() == WordType::number) {
                Stack.push(exprPart);
            }
            //todo: memory lost
            else if (exprPart->getType() == WordType::op) {
                assert(!Stack.empty());
                Number* secondArg = static_cast<Number*>(Stack.top()); Stack.pop();
                Number* firstArg = static_cast<Number*>(Stack.top()); Stack.pop();
                Operator* op = static_cast<Operator*>(exprPart);
                Number result = op->eval(*firstArg, *secondArg); 
                Stack.push(new Number{result});
            }
       }
       return static_cast<Number*>(Stack.top()); 
    }
};

int main() {
    std::string input;
    std::getline(std::cin, input);
    std::vector<Token> tokens = TokenMaker::tokenize(input); 
    std::vector<Token> rpn = getRPN(tokens);
    for (Token& token:  rpn) {
        std::cout << token.str;
    }
    std::cout << std::endl;
    std::vector<ExprPart*> words = WordFactory::convertToWords(rpn);
    Number* result = Processor::eval(words);
    if (result)
        std::cout << input << " = " << result->getValue() << std::endl;

    return 0;
}
