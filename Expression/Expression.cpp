#include "Expression.h"
#include <cassert>
#include <list>

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
    else if (token == "-") {
        return new BinMinus{};
    }
    else if (token == "*") {
        return new Mult{};
    }
    else if (token == "/") {
        return new Divide{};
    }
    return nullptr;
}

Number BinPlus::eval(Number* lhs, Number* rhs) {
    return lhs->getValue() + rhs->getValue();
}

Number BinMinus::eval(Number* lhs, Number* rhs) {
    return lhs->getValue() - rhs->getValue();
}

Number Mult::eval(Number* lhs, Number* rhs) {
    return lhs->getValue() * rhs->getValue();
}

Number Divide::eval(Number* lhs, Number* rhs) {
    return lhs->getValue() / rhs->getValue();
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

bool Scanner::_getFirstWord(const std::string& input, std::string& word, ExprPartType& type) {
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

bool Scanner::getFirstWord(const std::string& input, std::string& word, ExprPartType& type) {
    static const std::string operators = "+-*/";

    int i = 0;
    for(;'0' <= input[i] && input[i] <= '9'; ++i);
    if (i) {
        word = input.substr(0, i);
        type = ExprPartType::number;
        return true;
    }

    if (operators.find(input.front()) != std::string::npos) {
        word = input.front();
        type = ExprPartType::oper;
        return true;
    }

    return false;	
}

std::ostream& operator<<(std::ostream& os, const BinPlus& binPlus) {
    return os << '+';
}

std::ostream& operator<<(std::ostream& os, const BinMinus& binMinus) {
    return os << '-';
}

std::ostream& operator<<(std::ostream& os, const Mult& mult) {
    return os << '*';
}

std::ostream& operator<<(std::ostream& os, const Divide& div) {
    return os << '/';
}

std::ostream& operator<<(std::ostream& os, const ExprPart& exp) {
    if (const Number* ptr = dynamic_cast<const Number*>(&exp)) {
        os << *ptr;
    }
    else if (const BinPlus* ptr = dynamic_cast<const BinPlus*>(&exp)) {
        os << *ptr;
    }
    else if (const BinMinus* ptr = dynamic_cast<const BinMinus*>(&exp)) {
        os << *ptr;
    }
    else if (const Mult* ptr = dynamic_cast<const Mult*>(&exp)) {
        os << *ptr;
    }   
    else if (const Divide* ptr = dynamic_cast<const Divide*>(&exp)) {
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
    std::stack<std::unique_ptr<ExprPart>> stack;
    for (std::unique_ptr<ExprPart>& current: expression) {
        if (dynamic_cast<Number*>(current.get())) {
            rpn.push_back(std::move(current));
        }
        if (auto rawCurrent = dynamic_cast<BinOperator*>(current.get())) {
            while (!stack.empty()) {
                std::unique_ptr<ExprPart>& top = stack.top();
                auto rawTop = dynamic_cast<BinOperator*>(top.get());
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
        if (dynamic_cast<LPar*>(current.get())) {
            rpn.push_back(std::move(current));
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
Number eval(Expression& rpn) {
    std::stack<ExprPart*> stack;
    //Using list here because I need storage pointers on his elements in stack and vector invalidates pointers.
    std::list<Number> tmpNumbers;
    for (auto& current: rpn) {
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
    }
    return tmpNumbers.back();
}
