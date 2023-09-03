#include <iostream>
#include <memory>

class Expression {
public:
    virtual bool interpret(const std::string& context) = 0;
    virtual ~Expression() {} 
};

using ExpressionPtr = std::shared_ptr<Expression>;

class TerminalExpression: public Expression {
public:
    TerminalExpression(const std::string& data): data_(data) {}
    bool interpret(const std::string& context) {
        auto n = context.find(data_);
        if (n != std::string::npos) {
            return true;
        }
        else {
            return false;
        }
    }
private:
    std::string data_;
};

class OrExpression: public Expression {
public:
    OrExpression(const ExpressionPtr& first, const ExpressionPtr& second)
    : first_(first), second_(second) {}
    bool interpret(const std::string& context) {
        return first_->interpret(context) || second_->interpret(context);
    }
private:
    ExpressionPtr first_;
    ExpressionPtr second_;
};

class AndExpression: public Expression {
public:
    AndExpression(const ExpressionPtr& first, const ExpressionPtr& second)
    : first_(first), second_(second) {}
    bool interpret(const std::string& context) {
        return first_->interpret(context) && second_->interpret(context);
    }
private:
    ExpressionPtr first_;
    ExpressionPtr second_;
};

ExpressionPtr getMaleExpr() {
    ExpressionPtr first = std::make_shared<TerminalExpression>("John");
    ExpressionPtr second = std::make_shared<TerminalExpression>("Robert");
    return std::make_shared<OrExpression>(first, second);
}

ExpressionPtr getMarriedWomanExpr() {
    ExpressionPtr first = std::make_shared<TerminalExpression>("Julie");
    ExpressionPtr second = std::make_shared<TerminalExpression>("Married");
    return std::make_shared<AndExpression>(first, second);
}

int main() {
    ExpressionPtr isMale = getMaleExpr();
    ExpressionPtr isMarried = getMarriedWomanExpr();
    std::cout << std::boolalpha << isMale->interpret("John") << std::endl;
    std::cout << std::boolalpha << isMarried->interpret("Julie Married") << std::endl;
    return 0;
}
