#include <iostream>
#include <memory>

class Image {
public:
    virtual void display() = 0;
    virtual ~Image() {}
};

class RealImage: public Image {
public:
    RealImage(const std::string& fileName): fileName_(fileName) {
        loadFromDisk(fileName);
    }

    void display() override {
        std::cout << "Displaying " << fileName_ << std::endl;
    }

    void loadFromDisk(const std::string& fileName) {
        std::cout << "Loading " << fileName_ << std::endl;
    }
private:
    std::string fileName_;
};

class ProxyImage: public Image {
public:
    ProxyImage(const std::string& fileName): fileName_(fileName) {
    }

    void display() override {
        if (!realImage_) 
            realImage_.reset(new RealImage{fileName_});
        realImage_->display();
    }

private:
    std::string fileName_;
    std::unique_ptr<RealImage> realImage_;
};

int main() {
    std::unique_ptr<Image> image = std::make_unique<ProxyImage>("test.jpg");
    image->display();
    std::cout << std::endl;
    image->display();

    return 0;
}

