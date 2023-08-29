#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <random>

class Shape {
public:
    virtual void draw() = 0;
};

using ShapePtr = std::shared_ptr<Shape>;


class Circle: public Shape {
public:
    Circle(const std::string& color): color_(color) {}

    void setX(int x) { x_ = x; }
    void setY(int y) { y_ = y; }
    void setR(int r) { r_ = r; }

    void draw() override {
        std::cout << "color: " << color_
                  << " x: " << x_ 
                  << " y: " << y_
                  << " r: " << r_ << "\n";
    }

private:
    int x_;
    int y_;
    int r_;
    std::string color_;
};

class ShapeFactory {
public:
    ShapePtr getCircle(const std::string& color) {
        auto search = circles_.find(color);
        if (search != circles_.end()) {
            return search->second;
        }
        else {
            circles_[color] = std::make_shared<Circle>(color); // ??? a = (b = c)
            std::cout << "Creating circle of color: " << color << "\n";
            return circles_[color];
        }
    }
private:
    std::unordered_map<std::string, ShapePtr> circles_;
};





int main() {
    std::random_device dev;
    std::mt19937 rng(dev());

    const std::vector<std::string> colors{"red", "green", "blue", "white", "black"};
    int h = 100;
    int w = 100;

    std::uniform_int_distribution<int> distX(0, w);
    std::uniform_int_distribution<int> distY(0, h);
    std::uniform_int_distribution<int> distColors(0, colors.size() - 1);

    ShapeFactory factory;

    for (int i = 0; i < 20; ++i) {
        auto color = colors[distColors(rng)];
        auto circle = static_cast<Circle&>(*factory.getCircle(color));
        circle.setX(distX(rng));
        circle.setY(distY(rng));
        circle.setR(100);
        circle.draw();
    }

    return 0;
}
