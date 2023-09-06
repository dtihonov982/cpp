#include <iostream>
#include <memory>

class Strategy;
using StrategyPtr = std::unique_ptr<Strategy>;

class Strategy {
public:
    virtual int doOperation(int x, int y) = 0;
    virtual StrategyPtr clone() const = 0;
    virtual ~Strategy() {}
};

class OperationAdd: public Strategy {
public:
    int doOperation(int lhs, int rhs) override {
        return lhs + rhs;
    }
    StrategyPtr clone() const {
        return std::make_unique<OperationAdd>(*this);
    }
};


class OperationSubstract: public Strategy {
public:
    int doOperation(int lhs, int rhs) override {
        return lhs - rhs;
    }
    StrategyPtr clone() const {
        return std::make_unique<OperationSubstract>(*this);
    }
};

class Context {
public:
    Context(const Strategy& strategy): strategy_(strategy.clone()) {
    }

    int executeStrategy(int a, int b) {
        return strategy_->doOperation(a, b);
    }
private:
    StrategyPtr strategy_;
};

int main() {
    Context context(OperationAdd{});
    std::cout << context.executeStrategy(10, 5) << std::endl;
    context = Context(OperationSubstract{});
    std::cout << context.executeStrategy(10, 5) << std::endl;
    return 0;
}
