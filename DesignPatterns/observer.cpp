#include <iostream>
#include <vector>
#include <memory>
#include <bitset>

class Subject; 

class Observer {
public:
    virtual void update(Subject& subject) = 0;
};

using ObserverPtr = std::unique_ptr<Observer>;

class Subject {
public:
    int getState() { return state_; }
    void setState(int state) {
        state_ = state;
        notifyAll();
    }
    void attach(ObserverPtr&& observer) {
        observers_.push_back(std::move(observer));
    }
    void notifyAll() {
        for (auto& ob: observers_) {
            ob->update(*this);
        }
    }
private:
    int state_;
    std::vector<ObserverPtr> observers_;
};


class HexaObserver: public Observer {
public:
    void update(Subject& subj) override {
        std::cout << std::hex << subj.getState() << std::endl;
    }
};

class OctalObserver: public Observer {
public:
    void update(Subject& subj) override {
        std::cout << std::oct << subj.getState() << std::endl;
    }
};

class BinObserver: public Observer {
public:
    void update(Subject& subj) override {
        std::cout << std::bitset<32>(subj.getState()) << std::endl;
    }
};

template <typename T>
void attach(Subject& subject) {
    std::unique_ptr<Observer> tmp = std::make_unique<T>();
    subject.attach(std::move(tmp));
}

int main() {
    Subject subject;
    attach<HexaObserver>(subject); 
    attach<OctalObserver>(subject); 
    attach<BinObserver>(subject); 
    subject.setState(15);
    subject.setState(10);
    return 0;
}
