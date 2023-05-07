#include "Expression.h"
#include <cassert>

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


Number Sqr::eval(const std::vector<const Number*>& argv) { 
    auto value = argv[0]->getValue();
    return Number{value * value};
} 

Function* Function::makeFunction(const std::string& token) {
    if (token == "sqr") {
        return new Sqr{};
    }
    return nullptr;
}
