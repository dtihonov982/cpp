#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class ExprPart;

using Expression = std::vector<std::unique_ptr<ExprPart>>;

enum class ExprPartType { oper, func, number, lpar, rpar, ref };

std::ostream& operator<<(std::ostream& os, ExprPartType type);

class ExprPart {
public:
    virtual ~ExprPart() = default;
};

class Number: public ExprPart {
public:
    Number(int n): value(n) {}
    int getValue() const { return value; }
private:
    int value;
};

std::ostream& operator<<(std::ostream& os, const Number& n);

struct RPar: public ExprPart {
};

std::ostream& operator<<(std::ostream& os, const RPar& rpar);

struct LPar: public ExprPart {
};

std::ostream& operator<<(std::ostream& os, const LPar& lpar);

using precedence_t = unsigned char;

class BinOperator: public ExprPart {
public:
    static BinOperator* makeBinOperator(std::string token);
    virtual Number eval(Number* lhs, Number* rhs) = 0;
    virtual precedence_t getPrecedence() = 0;
    virtual ~BinOperator() = default;
};

class BinPlus: public BinOperator {
public:
    Number eval(Number* lhs, Number* rhs) override;
    precedence_t getPrecedence() override { return 2; }
};

class BinMinus: public BinOperator {
public:
    Number eval(Number* lhs, Number* rhs) override;
    precedence_t getPrecedence() override { return 2; }
};

class Mult: public BinOperator {
public:
    Number eval(Number* lhs, Number* rhs) override;
    precedence_t getPrecedence() override { return 3; }
};

class Divide: public BinOperator {
public:
    Number eval(Number* lhs, Number* rhs) override;
    precedence_t getPrecedence() override { return 3; }
};

std::ostream& operator<<(std::ostream& os, const BinPlus& binPlus);

std::ostream& operator<<(std::ostream& os, const BinMinus& binMinus);

std::ostream& operator<<(std::ostream& os, const ExprPart& exp);

std::ostream& operator<<(std::ostream& os, const Mult& mult);

std::ostream& operator<<(std::ostream& os, const Divide& div);

class Function: public ExprPart {
public:
    virtual Number eval(const std::vector<const Number*>& argv) = 0;
    virtual int argc() = 0;
    static Function* makeFunction(const std::string& token);
    virtual ~Function() = default;
};

class Sqr: public Function {
public:
    Number eval(const std::vector<const Number*>& argv) override;
    int argc() { return 1; }
};

#endif
