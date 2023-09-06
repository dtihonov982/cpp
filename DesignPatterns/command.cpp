#include <iostream>
#include <memory>
#include <vector>

class Order {
public:
    virtual void execute() = 0;
    virtual ~Order() {}
};

using OrderPtr = std::unique_ptr<Order>;

class Stock {
public:
    Stock(const std::string& name, int quantity) 
    : name_(name), quantity_(quantity) {} 
    void buy() {
        std::cout << "Buy " << name_ << " " << quantity_ << std::endl;
    }
    void sell() {
        std::cout << "Sell " << name_ << " " << quantity_ << std::endl;
    }
private:
    std::string name_;
    int quantity_;
};

class BuyStock: public Order {
public:
    BuyStock(const Stock& stock): stock_(stock) {}
    void execute() override {
        stock_.buy();
    }
private:
    Stock stock_;
};

class SellStock: public Order {
public:
    SellStock(const Stock& stock): stock_(stock) {}
    void execute() override {
        stock_.sell();
    }
private:
    Stock stock_;
};

class Broker {
public:
    template <typename OrderType>
    void takeOrder(const OrderType& order) {
        orders_.emplace_back(new OrderType(order));
    }
    void placeOrders() {
        for (auto& order: orders_)
            order->execute();
        orders_.clear();
    }
private:
    std::vector<OrderPtr> orders_;
};

int main() {
    Stock bmw("BMW", 10);
    Stock volvo("Volvo", 20);
    Broker broker;
    broker.takeOrder(BuyStock(bmw));
    broker.takeOrder(SellStock(volvo));
    broker.takeOrder(SellStock(bmw));

    broker.placeOrders();
    return 0;
}
