#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Memento {
public:
    Memento(const std::string& state): state_(state) {}
    std::string getState() const { return state_; }
private:
    std::string state_;
};

class Originator {
public:
    void setState(const std::string& state) {
        state_ = state;
    }
    std::string getState() {
        return state_;
    }
    Memento saveStateToMemento() {
        return Memento(state_);
    }
    void getStateFromMemento(const Memento& memento) {
        state_ = memento.getState();
    }
private:
    std::string state_;
};

class CareTaker {
public:
    void add(const Memento& state) {
        mementoList_.push_back(state);
    }

    void add(Memento&& state) {
        mementoList_.push_back(std::move(state));
    }

    Memento get(size_t index) {
        return mementoList_[index];
    }
private:
    std::vector<Memento> mementoList_;
};

int main() {
    Originator originator;
    CareTaker careTaker;

    originator.setState("State #1");
    originator.setState("State #2");
    careTaker.add(originator.saveStateToMemento());

    originator.setState("State #3");
    careTaker.add(originator.saveStateToMemento());

    originator.setState("State #4");
    std::cout << originator.getState() << std::endl;

    originator.getStateFromMemento(careTaker.get(0));
    std::cout << originator.getState() << std::endl;

    originator.getStateFromMemento(careTaker.get(1));
    std::cout << originator.getState() << std::endl;

    return 0;
}
