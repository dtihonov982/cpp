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




#if 0
class Word {
public:
    WordType getType() { return type; }
    virtual ~Word();
private:
    WordType type;
};

class Operator: public Word {
public:
    Operator(const Token& token_, int precedence_): type(WordType::op), token(token_), precedence(precedence_) {}
    int getPrecedence() { return precedence; }
private:
    int precedence;
    Token token;
};

class Number: public Word {
public:
    Number(const Token& token): type(WordType::number) {
        value = std::stoi(token);
    }
    int getValue() { return value; }
private:
    int value;
};

class LPar: public Word {
public:
    LPar(): type(WordType::LPar) {}
};

class RPar: public Word {
public:
    RPar(): type(WordType::RPar) {}
};

class WordFactory {
public:
    static Word* makeWord(const Token& token) {
        if (auto op = operators.find(token); token != operators.end()) {
            return new Operator{op->first, op->second};
        }
        else if (token == "(") {
            return new LPar{};
        }
        else if (token == ")") {
            return new RPar{};
        }
        else {
            return new Number{token};
        }
    }
private:
    std::map<Token, int> operators = { {"+", 2}, {"-", 2}, {"*", 3}, {"/", 3} };
};

std::queue<Word*> getRPN(const std::vector<Token>& input) {
    std::queue<Word*> output;
    std::stack<Word*> opStack;
    for (const Token& token: input) {
        Word* currWord = WordFactory::makeWord(token);
        switch (currWord::getType()) {

        case WordType::number:
            output.push_back(currWord);
            break;

        case WordType::op:
            while (!opStack.empty()) {
                Word* topWord = opStack.top();
                if (topWord->getType() != WordType::Lpar && topWord->getPrecedence() >= currWord->getPrecedence()) {
                    output.push_back(topWord);
                    opStack.pop();
                }
                else {
                    break;
                }
            }
            opStack.push(currWord);
            break;

        case WordType::LPar:
            opStack.push(currWord);
            break;

        case WordType::RPar:
            while (!opStack.empty() && OpStack.top()->getType() != WordType::LPar) {
                output.push_back(opStack.top());
                opStack.pop();
            }
            if (opStack.top() == WordType::LPar) {
                opStack.pop();
            }
            break;
        }
    }
    while (!opStack.empty()) {
        output.push_back(opStack.top()); opStack.pop();
    }
    return output;
}
#endif

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

#if 0
std::vector<Token> tokenize(const std::string& input) {
    std::istringstream iss{input};
    std::string tmp;
    std::vector<std::string> result;
    while (std::getline(iss, tmp, ' ')) {
        result.push_back(tmp);
    }
    return result;
}
#endif

int main() {
    std::string input;
    std::getline(std::cin, input);
    for (Token& token: TokenMaker::tokenize(input) ) {
        std::cout << token << std::endl;
    }
    #if 0
    auto tokens = tokenize(input);
    for (auto x: tokens) {
        std::cout << "{" << x << "}, ";
    }
    std::cout << std::endl;
    //std::queue<Word*> rpn = getRPN(input);
    #endif

    return 0;
}
