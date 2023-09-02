#include <iostream>
#include <memory>

class Context; 

class State {
public:
    virtual void doAction(Context& context) = 0;
    virtual std::string toString() = 0;
    virtual ~State() {}
};

using StatePtr = std::shared_ptr<State>;

class Context {
public:
    StatePtr getState() { return state_; }
    void setState(const StatePtr& state) { state_ = state; }
private:
    StatePtr state_;
};

class StartState: public State {
public:
    void doAction(Context& context) override {
        std::cout << "Context in start state.\n";
        context.setState(clone());
    }
    std::shared_ptr<StartState> clone() { return std::make_shared<StartState>(*this); }
    std::string toString() { return "start state"; }
};

class StopState: public State {
public:
    void doAction(Context& context) override {
        std::cout << "Context in stop state.\n";
        context.setState(clone());
    }
    std::shared_ptr<StopState> clone() { return std::make_shared<StopState>(*this); }
    std::string toString() { return "stop state"; }
};


int main() {
    Context context;
    StatePtr start = std::make_unique<StartState>();
    start->doAction(context);
    std::cout << context.getState()->toString() << std::endl;

    StatePtr stop = std::make_unique<StopState>();
    stop->doAction(context);
    std::cout << context.getState()->toString() << std::endl;

    return 0;
}
