#include <string>
#include <regex>
#include <utility>
#include <iostream>
#include <memory>
#include <stack>
#include <utility>

class ExprPart;

using Expression = std::vector<std::unique_ptr<ExprPart>>;

enum class ExprPartType { oper, func, number, lpar, rpar, ref };

std::ostream& operator<<(std::ostream& os, ExprPartType type);

class ExprPart {
public:
    virtual ~ExprPart() {}
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
};

class BinPlus: public BinOperator {
public:
    Number eval(Number* lhs, Number* rhs) override;
    precedence_t getPrecedence() override;
};

std::ostream& operator<<(std::ostream& os, const BinPlus& binPlus);

std::ostream& operator<<(std::ostream& os, const ExprPart& exp);

class Scanner {
public:
	static const std::vector<std::pair<ExprPartType, std::regex>> regexes;
	static const int maxLengthOfWord;
	static bool getFirstWord(const std::string& input, std::string& word, ExprPartType& type);
	static Expression getExpression(std::string input);
    static void convertToRPN(Expression& expression);
};

Number eval(Expression& rpn);

