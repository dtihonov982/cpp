#include "Scanner.h"
#include <algorithm>
#include <cassert>

bool Scanner::getFirstWord(const std::string& input, std::string& word, ExprPartType& type) {
    assert(!input.empty());
    static const std::string operators = "+-*/";

    int i = 0;
    for(;'0' <= input[i] && input[i] <= '9'; ++i);
    if (i) {
        word = input.substr(0, i);
        type = ExprPartType::number;
        return true;
    }

    for(;'A' <= input[i] && input[i] <= 'Z'; ++i);
    if (i) {
        word = input.substr(0, i);
        type = ExprPartType::func;
        return true;
    }


    if (operators.find(input.front()) != std::string::npos) {
        word = input.front();
        type = ExprPartType::oper;
        return true;
    }

    if (input[0] == '(') {
        word = "(";
        type = ExprPartType::lpar;
        return true;
    }

    if (input[0] == ')') {
        word = ")";
        type = ExprPartType::rpar;
        return true;
    }

    return false;	
}

const int Scanner::maxLengthOfWord = 255;

Expression Scanner::getExpression(std::string input) {		
    //deleting spaces
    input.erase(std::remove_if(input.begin(), input.end(), [] (char x) { return x == ' '; }), input.end());
    std::transform(input.begin(), input.end(), input.begin(), ::toupper);
    Expression result;
    std::string word;
    ExprPartType type;
    int value;
    for (int i = 0, recognizedWordLength; i < input.size(); i += recognizedWordLength) {
        //getting short part of whole string that may be too long for fast regex matching
        std::string current = input.substr(i, maxLengthOfWord);
        if (getFirstWord(current, word, type)) {
            recognizedWordLength = word.length();
            switch (type) {
                case ExprPartType::number:
                    value = std::stoi(word);
                    result.emplace_back(new Number{value});
                    break;
                case ExprPartType::lpar:
                    result.emplace_back(new LPar{});
                    break;
                case ExprPartType::rpar:
                    result.emplace_back(new RPar{});
                    break;
                case ExprPartType::oper: {
                    BinOperator* oper = BinOperator::makeBinOperator(word);
                    result.emplace_back(oper);
                    break; 
                }
                case ExprPartType::func: {
                    Function* f = Function::makeFunction(word);
                    result.emplace_back(f);
                    break; 
                }
            }
        }
        else {
            throw std::invalid_argument("Can`t recognize \"" + current + "\"");
        }
    }
    return result;
}
