#include <iostream>
#include <vector>
#include <memory>

//TODO: return base class ptr from method decorated in unique_ptr
//TODO: partitial interface immplementation in inheritance hierarchy

class Packing;
using PackingPtr = std::unique_ptr<Packing>;

class Item {
public:
    virtual std::string name() = 0;
    virtual PackingPtr packing() = 0;
    virtual float price() = 0;
    virtual ~Item() {}
};

using ItemPtr = std::unique_ptr<Item>;

class Packing  {
public:
    virtual std::string pack() = 0;
    virtual ~Packing() {}
};

class Wrapper: public Packing {
public:
    std::string pack() override {
        return "Wrapper";
    }

};

class Bottle: public Packing {
public:
    std::string pack() override {
        return "Bottle";
    }
};

class Burger: public Item {
public:
    PackingPtr packing() override {
        return PackingPtr{new Wrapper()};
    }
    virtual ~Burger() {}
};

class ColdDrink: public Item {
public:
    PackingPtr packing() override {
        return PackingPtr{new Bottle()};
    }
    virtual ~ColdDrink() {}
};

class VegBurger: public Burger {
public:
    float price() override {
        return 25.0f;
    }
    
    std::string name() override {
        return "Veg Burger";
    }
};

class ChickenBurger: public Burger {
public:
    float price() override {
        return 50.5f;
    }
    
    std::string name() override {
        return "Chicken Burger";
    }
};

class Coke: public ColdDrink {
public:
    float price() override {
        return 30.0f;
    }

    std::string name() override {
        return "Coke";
    }
};

class Pepsi: public ColdDrink {
public:
    float price() override {
        return 35.0f;
    }

    std::string name() override {
        return "Pepsi";
    }
};

class Meal {
private:
    std::vector<ItemPtr> items;
public:
    void addItem(Item* item) {
        items.emplace_back(item);
    }

    float getCost() {
        float cost = 0.0f;
        for (ItemPtr& item: items) {
            cost += item->price();
        }
        return cost;
    }

    void showItems() {
        for (ItemPtr& item: items) {
            std::cout << "Item: " << item->name() << '\n';
            std::cout << "Packing: " << item->packing()->pack() << '\n';
            std::cout << "Price: " << item->price() << '\n';
        }
    }
};

class MealBuilder {
public:
   Meal prepareVegMeal() {
       Meal meal;
       meal.addItem(new VegBurger{});
       meal.addItem(new Coke{});
       return meal;
   }
   Meal prepareMeal() {
       Meal meal;
       meal.addItem(new ChickenBurger{});
       meal.addItem(new Pepsi());
       return meal;
   }
};

int main() {
    MealBuilder mealBuilder;
    Meal veg = mealBuilder.prepareVegMeal();
    std::cout << "Veg meal\n";
    veg.showItems();
    std::cout << "Total cost: " << veg.getCost();

    Meal meal = mealBuilder.prepareMeal();
    std::cout << "Meal\n";
    meal.showItems();
    std::cout << "Total cost: " << meal.getCost();

    return 0;
}
