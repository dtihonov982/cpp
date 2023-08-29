#include <unordered_map>
#include <iostream>
#include <memory>

class Shape;
using ShapePtr = std::unique_ptr<Shape>;

class Shape {
public:
    virtual void draw() = 0;
    virtual ~Shape() {}
};


class Rectangle: public Shape {
public:
    void draw() override {
        std::cout << "Inside Rectangle::draw() method.\n";
    }
};

class Circle: public Shape {
public:
    void draw() override {
        std::cout << "Inside Circle::draw() method.\n";
    }
};

class ShapeDecorator: public Shape {
protected:
    ShapePtr decoratedShape_;
public:
    ShapeDecorator(ShapePtr&& decoratedShape)
    : decoratedShape_(std::move(decoratedShape)) {
    }
    void draw() override {
        decoratedShape_->draw();
    }
    ~ShapeDecorator() {}
};

class RedShapeDecorator: public ShapeDecorator {
public:
    RedShapeDecorator(ShapePtr&& decoratedShape)
    : ShapeDecorator(std::move(decoratedShape)) {
    }
    void draw() override {
        decoratedShape_->draw();
        setRedBorder();
    }
    void setRedBorder() {
        std::cout << "Border color: red.\n";
    }
};


int main() {
    ShapePtr circle(new Circle());
    ShapePtr redCircle = std::make_unique<RedShapeDecorator>(std::make_unique<Circle>());
    ShapePtr redRectangle = std::make_unique<RedShapeDecorator>(std::make_unique<Rectangle>());


    std::cout << "Circle with normal border " << '\n';
    circle->draw();

    std::cout << "Circle of red border " << '\n';
    redCircle->draw();

    std::cout << "Rectangle of red border " << '\n';
    redRectangle->draw();

    return 0;
}


