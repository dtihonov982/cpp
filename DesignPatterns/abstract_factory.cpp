#include <iostream>
#include <string_view>
#include <memory>

class Shape {
public:
    virtual void draw() = 0;
};

using ShapePtr = std::unique_ptr<Shape>;

class Circle: public Shape {
public:
    void draw() override {
        std::cout << "Circle::draw\n";
    }
};

class Square: public Shape {
public:
    void draw() override {
        std::cout << "Square::draw\n";
    }
};

class RoundedCircle: public Shape {
public:
    void draw() override {
        std::cout << "RoundedCircle::draw\n";
    }
};

class RoundedSquare: public Shape {
public:
    void draw() override {
        std::cout << "RoundedSquare::draw\n";
    }
};

class AbstractFactory {
public:
    virtual ShapePtr getShape(std::string_view tag) = 0;
};

using AbstractFactoryPtr = std::unique_ptr<AbstractFactory>;

class ShapeFactory: public AbstractFactory {
public:
    ShapePtr getShape(std::string_view tag) override {
        if (tag == "circle") {
            return std::make_unique<Circle>();
        }
        else if (tag == "square") {
            return std::make_unique<Square>();
        }
        else {
            return {};
        }
    }
};

class RoundedShapeFactory: public AbstractFactory {
public:
    ShapePtr getShape(std::string_view tag) override {
        if (tag == "circle") {
            return std::make_unique<RoundedCircle>();
        }
        else if (tag == "square") {
            return std::make_unique<RoundedSquare>();
        }
        else {
            return {};
        }
    }
};

class FactoryProducer {
public:
    AbstractFactoryPtr getFactory(bool rounded) {
        if (rounded) {
            return std::make_unique<RoundedShapeFactory>();
        }
        else {
            return std::make_unique<ShapeFactory>();
        }
    }
};

int main() {
    FactoryProducer prod;
    AbstractFactoryPtr f1 = prod.getFactory(false);
    ShapePtr s1 = f1->getShape("circle");
    s1->draw();

    ShapePtr s2 = f1->getShape("square");
    s2->draw();

    AbstractFactoryPtr f2 = prod.getFactory(true);
    ShapePtr s3 = f2->getShape("circle");
    s3->draw();

    ShapePtr s4 = f2->getShape("square");
    s4->draw();

    return 0;
}

