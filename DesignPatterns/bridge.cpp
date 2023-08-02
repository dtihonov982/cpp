#include <iostream>
#include <memory>

class DrawAPI {
public:
    virtual void drawCircle(int radius, int x, int y) = 0;
    virtual ~DrawAPI() {}
};


class RedCircle: public DrawAPI {
public:
    void drawCircle(int radius, int x, int y) override {
        std::cout << "Drawing circle [color: reg, radius: "
                  << radius << ", x: " << x << " , y: " << y << "]\n";
    }
};

class GreenCircle: public DrawAPI {
public:
    void drawCircle(int radius, int x, int y) override {
        std::cout << "Drawing circle [color: green, radius: "
                  << radius << ", x: " << x << " , y: " << y << "]\n";
    }
};

class Shape {
public:
    virtual void draw() = 0;
    virtual ~Shape() {}
protected:
    DrawAPI* drawAPI_;
    Shape(DrawAPI* drawAPI) {
        drawAPI_ = drawAPI;
    }
};

typedef std::unique_ptr<Shape> ShapePtr;

class Circle: public Shape {
public:
    Circle(int x, int y, int radius, DrawAPI* drawAPI)
    : Shape(drawAPI)
    , x_(x), y_(y), radius_(radius) {
    }

    void draw() override {
        drawAPI_->drawCircle(radius_, x_, y_);
    }
private:
    int x_;
    int y_;
    int radius_;
};

int main() {
    ShapePtr redCircle(new Circle(101, 102, 10, new RedCircle()));
    ShapePtr greenCircle(new Circle(201, 202, 20, new GreenCircle()));
    redCircle->draw();
    greenCircle->draw();
    return 0;
}
