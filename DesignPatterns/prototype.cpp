#include <unordered_map>
#include <iostream>
#include <memory>

class Shape;
using ShapePtr = std::unique_ptr<Shape>;

class Shape {
public:
    virtual void draw() = 0;
    std::string getType() { return type_; }
    std::string getId() { return id_; }
    void setId(std::string id) { id_ = id; }
    virtual ShapePtr clone() = 0;
    virtual ~Shape() {}
protected:
    std::string type_;
private:
    std::string id_;
};


class Rectangle: public Shape {
public:
    Rectangle() {
        type_ = "Rectangle";
    }
    void draw() override {
        std::cout << "Inside Rectangle::draw() method.\n";
    }
    ShapePtr clone() override {
        Rectangle* tmp = new Rectangle{*this};
        return ShapePtr{tmp};
    }
};

class Square: public Shape {
public:
    Square() {
        type_ = "Square";
    }
    void draw() override {
        std::cout << "Inside Square::draw() method.\n";
    }
    ShapePtr clone() override {
        Square* tmp = new Square{*this};
        return ShapePtr{tmp};
    }
};

class Circle: public Shape {
public:
    Circle() {
        type_ = "Circle";
    }
    void draw() override {
        std::cout << "Inside Circle::draw() method.\n";
    }
    ShapePtr clone() override {
        Circle* tmp = new Circle{*this};
        return ShapePtr{tmp};
    }
};

class ShapeCache {
public:
    ShapePtr getShape(std::string id) {
        auto search = shapeMap.find(id);
        if (search != shapeMap.end()) {
            return search->second->clone();
        }
        else {
            return {};
        }
    }
    
    void loadCache() {
        Circle* circle = new Circle;
        circle->setId("001");
        shapeMap.emplace(circle->getId(), circle);

        Rectangle* rec = new Rectangle;
        rec->setId("002");
        shapeMap.emplace(rec->getId(), rec);

        Square* square = new Square;
        square->setId("003");
        shapeMap.emplace(square->getId(), square);
    }
private:
    std::unordered_map<std::string, ShapePtr> shapeMap;
};


int main() {
    ShapeCache cache;
    cache.loadCache();

    ShapePtr cloned1 = cache.getShape("001");
    std::cout << "Shape: " << cloned1->getType() << '\n';

    ShapePtr cloned2 = cache.getShape("002");
    std::cout << "Shape: " << cloned2->getType() << '\n';

    ShapePtr cloned3 = cache.getShape("003");
    std::cout << "Shape: " << cloned3->getType() << '\n';
    return 0;
}


