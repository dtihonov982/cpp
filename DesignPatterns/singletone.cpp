#include <iostream>

class SingleObject {
public:
    static SingleObject& getInstance() {
        return instance_;
    }
    void showMessage() {
        std::cout << "Hi!" << std::endl;
    }
private:
    SingleObject() {}
    static SingleObject instance_;
};

SingleObject SingleObject::instance_{};

int main() {
    // SingleObject error;
    SingleObject& object = SingleObject::getInstance();
    object.showMessage();

    return 0;
}
