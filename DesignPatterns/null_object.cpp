#include <iostream>
#include <memory>
#include <unordered_set>
#include <vector>

class AbstractCustomer {
public:
    virtual bool isNil() const = 0;
    virtual std::string getName() = 0;
    virtual ~AbstractCustomer() {}
protected:
    std::string name;
};

class RealCustomer: public AbstractCustomer {
public:
    RealCustomer(const std::string& name)
    : name_(name) {
    }

    std::string getName() override {
        return name_;
    }

    bool isNil() const override {
        return false;
    }
private:
    std::string name_;
};

class NullCustomer: public AbstractCustomer {
public:
    std::string getName() override {
        return "Not available.";
    }

    bool isNil() const override {
        return true;
    }
};

class CustomerFactory {
public:
    static std::unordered_set<std::string> names_;
    static std::unique_ptr<AbstractCustomer> getCustomer(const std::string& name) {
        auto search = names_.find(name);
        if (search != names_.end())
            return std::make_unique<RealCustomer>(*search);
        else
            return std::make_unique<NullCustomer>();
    }
};
std::unordered_set<std::string> CustomerFactory::names_ {"Rob","Joe", "Julie"};

int main() {
    std::vector<std::unique_ptr<AbstractCustomer>> customers;
    auto names  = {"Rob", "Bob", "Julie", "Laura"};

    for (const auto& name: names) {
        customers.emplace_back(CustomerFactory::getCustomer(name));
    }

    for (auto& customer: customers) {
        std::cout << customer->getName() << std::endl;
    }

    return 0;
}
