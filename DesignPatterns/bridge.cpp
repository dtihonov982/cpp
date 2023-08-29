#include <iostream>
#include <memory>

class DrawAPI {
public:
    virtual void drawCircle(int radius, int x, int y) = 0;
    virtual ~DrawAPI() {}
};

using DrawAPIPtr = std::shared_ptr<DrawAPI>;


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
    DrawAPIPtr drawAPI_;
    Shape(DrawAPIPtr drawAPI) {
        drawAPI_ = drawAPI;
    }
};

typedef std::unique_ptr<Shape> ShapePtr;

class Circle: public Shape {
public:
    Circle(int x, int y, int radius, const DrawAPIPtr& drawAPI)
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
    auto red = std::make_shared<RedCircle>();
    ShapePtr redCircle(new Circle(101, 102, 10, red));
    auto green = std::make_shared<GreenCircle>();
    ShapePtr greenCircle(new Circle(201, 202, 20, green));
    redCircle->draw();
    greenCircle->draw();
    return 0;
}
