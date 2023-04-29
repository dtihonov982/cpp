#include <string>
#include <regex>
#include <utility>
#include <iostream>
#include <memory>

class ExprPart;

using Expression = std::vector<std::unique_ptr<ExprPart>>;

//TODO: dynamic cast
enum class ExprPartType { oper, func, number, lpar, rpar, ref };

std::ostream& operator<<(std::ostream& os, ExprPartType type);

class ExprPart {
public:
    ExprPart(ExprPartType type_): type(type_) {}
    ExprPartType getType() { return type; }
    virtual ~ExprPart() {}
private:
    ExprPartType type;
};

class Number: public ExprPart {
public:
    Number(int n): ExprPart(ExprPartType::number), value(n) {}
    int getValue() const { return value; }
private:
    int value;
};

std::ostream& operator<<(std::ostream& os, const Number& n);

struct RPar: public ExprPart {
    RPar(): ExprPart(ExprPartType::rpar) {}
};

std::ostream& operator<<(std::ostream& os, const RPar& rpar);

struct LPar: public ExprPart {
    LPar(): ExprPart(ExprPartType::lpar) {}
};

std::ostream& operator<<(std::ostream& os, const LPar& lpar);

class BinOperator: public ExprPart {
public:
    BinOperator(): ExprPart(ExprPartType::oper) {}
    virtual int eval(int lhs, int rhs) = 0;
};

class BinPlus: public BinOperator {
public:
    int eval(int lhs, int rhs) override;
};

class Scanner {
public:
	static const std::vector<std::pair<ExprPartType, std::regex>> regexes;
	static const int maxLengthOfWord;
	static bool getFirstWord(const std::string& input, std::string& word, ExprPartType& type);
	static Expression getExpression(std::string input);
};

