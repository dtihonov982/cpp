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

BinOperator* BinOperator::makeBinOperator(std::string token) {
    if (token == "+") {
        return new BinPlus{};
    }
    return nullptr;
}

int BinPlus::eval(int lhs, int rhs) {
    return lhs + rhs;
}

precedence_t BinPlus::getPrecedence() { return 2; }


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

std::ostream& operator<<(std::ostream& os, const BinPlus& binPlus) {
    return os << '+';
}

std::ostream& operator<<(std::ostream& os, const ExprPart& exp) {
    if (const Number* ptr = dynamic_cast<const Number*>(&exp)) {
        os << *ptr;
    }
    else if (const BinPlus* ptr = dynamic_cast<const BinPlus*>(&exp)) {
        os << *ptr;
    }
    else if (const LPar* ptr = dynamic_cast<const LPar*>(&exp)) {
        os << *ptr;
    }
    else if (const RPar* ptr = dynamic_cast<const RPar*>(&exp)) {
        os << *ptr;
    }
    else {
        os << "none";
    }
    return os;
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
                    BinOperator* oper = BinOperator::makeBinOperator(word);
                    result.emplace_back(oper);
                    break; 
            }
        }
        else {
            throw std::invalid_argument("Can`t recognize \"" + current + "\"");
        }
    }
    return result;
}




void Scanner::convertToRPN(Expression& expression) {
    Expression rpn;
    std::stack<ExprPart*> stack;
    for (std::unique_ptr<ExprPart>& exprPart: expression) {
        if (dynamic_cast<Number*>(exprPart.get())) {
            rpn.push_back(std::move(exprPart));
        }
        if (dynamic_cast<LPar*>(exprPart.get())) {
            rpn.push_back(std::move(exprPart));
        }
        #if 0
        if (auto ptr = dynamic_cast<BinOperator*>(exprPart.get())) {
            while (!stack.empty()) {
                ExprPart* top = stack.top();
                auto oper = dynamic_cast<BinOperator*>(top);
                if (oper && oper->getPrecedence() >= ptr->getPrecedence()) {
                    rpn.emplace_back(clone< 
                }
                else {
                    break;
                }
            }
        }
        #endif
    }
    expression = std::move(rpn);
}
