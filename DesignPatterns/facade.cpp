#include <iostream>

class Rectangle {
public:
    void draw() {
        std::cout << "Inside Rectangle::draw() method.\n";
    }
};

class Circle {
public:
    void draw() {
        std::cout << "Inside Circle::draw() method.\n";
    }
};

class Square {
public:
    void draw() {
        std::cout << "Inside Square::draw() method.\n";
    }
};

class Painter {
public:

    void drawCircle() { circle_.draw(); }

    void drawRectangle() { rect_.draw(); }

    void drawSquare() { square_.draw(); }

private:
    Rectangle rect_;
    Circle circle_;
    Square square_;
};

int main() {
    Painter painter;
    painter.drawCircle();
    painter.drawSquare();
    painter.drawRectangle();
    return 0;
}

