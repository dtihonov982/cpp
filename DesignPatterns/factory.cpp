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

class ShapeFactory {
public:
    ShapePtr getShape(std::string_view tag) {
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


int main() {
    ShapeFactory factory;
    ShapePtr s1 = factory.getShape("circle");
    s1->draw();

    ShapePtr s2 = factory.getShape("square");
    s2->draw();

    return 0;
}

