#include "Expression.h"

std::ostream& operator<<(std::ostream& os, ExprPartType type) {
    switch (type) {
    case ExprPartType::oper:
        os << "operator";
        break;
    case ExprPartType::func:
        os << "function";
        break;
    case ExprPartType::number:
        os << "number";
        break;
    case ExprPartType::lpar:
        os << "left parenthesis";
        break;
    case ExprPartType::rpar:
        os << "right parenthesis";
        break;
    case ExprPartType::ref:
        os << "reference";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Number& n) {
	os << n.getValue();
	return os;
}

std::ostream& operator<<(std::ostream& os, const RPar& rpar) {
	os << ')';
	return os;
}

std::ostream& operator<<(std::ostream& os, const LPar& lpar) {
	os << '(';
	return os;
}

int BinPlus::eval(int lhs, int rhs) {
    return lhs + rhs;
}

const int Scanner::maxLengthOfWord = 255;

const std::vector<std::pair<ExprPartType, std::regex>> Scanner::regexes = {
		{ ExprPartType::number, std::regex("^\\d+") }, 
		{ ExprPartType::oper, std::regex("^[\\+\\-\\*/]") }, 
		{ ExprPartType::lpar, std::regex("^\\(") }, 
		{ ExprPartType::rpar, std::regex("^\\)") }, 
		{ ExprPartType::func, std::regex("^\\w+") }, 
		{ ExprPartType::ref, std::regex("^\\[\\w+\\]") }
};

bool Scanner::getFirstWord(const std::string& input, std::string& word, ExprPartType& type) {
    std::smatch matches;
    for (auto& [currType, re]: regexes) {
        if (std::regex_search(input, matches, re)) {
            word = matches[0];
            type = currType;
            return true;
        }
    }
    return false;	
}

Expression Scanner::getExpression(std::string input) {		
    //deleting spaces
    input.erase(std::remove_if(input.begin(), input.end(), [] (char x) { return x == ' '; }), input.end());
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
                case ExprPartType::oper:
                   break; 
            }
        }
        else {
            throw std::invalid_argument("Can`t recognize \"" + current + "\"");
        }
    }
    return result;
}
