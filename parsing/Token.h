#include <iostream>
#include <stack>
#include <queue>
#include <string>
#include <sstream>
#include <regex>
#include <map>
#include <exception>
#include <algorithm>

enum class WordType { op, func, number, LPar, RPar, ref };

std::ostream& operator<<(std::ostream& os, WordType type) {
    switch (type) {
    case WordType::op:
        os << "operator";
        break;
    case WordType::func:
        os << "function";
        break;
    case WordType::number:
        os << "number";
        break;
    case WordType::LPar:
        os << "left parenthesis";
        break;
    case WordType::RPar:
        os << "right parenthesis";
        break;
    case WordType::ref:
        os << "reference";
        break;
    }
    return os;
}
//todo: Make ExpressionParts when recognize tokens. Tokenize must returns only ExpressionParts.
struct Token {
	std::string str;
	WordType type;
};

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.str << " " << token.type;
    return os;
}

class TokenMaker {
public:
    static std::vector<Token> tokenize(std::string input) {
        input.erase(std::remove_if(input.begin(), input.end(), [] (char x) { return x == ' '; }), input.end());
    	std::vector<Token> result;
        while (!input.empty()) {
            Token currToken;
            if (getTokenFromBegin(input, currToken)) {
                result.push_back(currToken);
                int lenToErase = currToken.str.size();
                input.erase(0, lenToErase);
            }
            else {
                throw std::runtime_error("Can't recognize: \"" + input + "\"");
            }
        }
    	return result;
    }

    static bool getTokenFromBegin(const std::string& input, Token& output) {
        std::smatch matches;
        for (int i = 0; i < res.size(); ++i) {
            if (std::regex_search(input, matches, res[i])) {
                output = {matches[0], types[i]};
                return true;
            }
        }
        return false;
    }
            
private:
    static std::vector<std::regex> res;
    static std::vector<WordType> types;
};

std::vector<std::regex> TokenMaker::res = {std::regex("^\\d+"), std::regex("^[\\+\\-\\*/]"), std::regex("^\\("), std::regex("^\\)"), std::regex("^\\w+"), std::regex("^\\[\\w+\\]")};
std::vector<WordType> TokenMaker::types = { WordType::number, WordType::op, WordType::LPar, WordType::RPar, WordType::func, WordType::ref};

int getOpPrecedence(const Token& opToken) {
    static std::map<std::string, int> operators { {"+", 2}, {"-", 2}, {"*", 3}, {"/", 3} };
    return operators[opToken.str];
}
//https://en.wikipedia.org/wiki/Shunting_yard_algorithm
std::vector<Token> getRPN(const std::vector<Token>& input) {
    std::vector<Token> output;
    std::stack<Token> tmpStack;
    for (const Token& token: input) {
        switch (token.type) {

        case WordType::number:
            output.push_back(token);
            break;

        case WordType::op:
            while (!tmpStack.empty()) {
                Token* top = &tmpStack.top();
                if (top->type != WordType::LPar && getOpPrecedence(*top) >= getOpPrecedence(token)) {
                    output.push_back(*top);
                    tmpStack.pop();
                }
                else {
                    break;
                }
            }
            tmpStack.push(token);
            break;

        case WordType::LPar:
            tmpStack.push(token);
            break;

        case WordType::RPar:
            while (!tmpStack.empty() && tmpStack.top().type != WordType::LPar) {
                output.push_back(tmpStack.top());
                tmpStack.pop();
            }
            if (tmpStack.top().type == WordType::LPar) {
                tmpStack.pop();
            }
            break;
        }
    }
    while (!tmpStack.empty()) {
        output.push_back(tmpStack.top()); tmpStack.pop();
    }
    return output;
}
