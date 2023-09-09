#include <iostream>
#include <memory>

class Game {
public:
    virtual void initialize() = 0;
    virtual void startPlay() = 0;
    virtual void endPlay() = 0;

    void play() {
        initialize();
        startPlay();
        endPlay();
    }
};

class Cricket: public Game {
public:
    void initialize() override {
        std::cout << "Cricket::initialize\n";
    }
    void startPlay() override {
        std::cout << "Cricket::startPlay\n";
    }
    void endPlay() override {
        std::cout << "Cricket::endPlay\n";
    }
};

class Football: public Game {
public:
    void initialize() override {
        std::cout << "Football::initialize\n";
    }
    void startPlay() override {
        std::cout << "Football::startPlay\n";
    }
    void endPlay() override {
        std::cout << "Football::endPlay\n";
    }
};

int main() {
    std::unique_ptr<Game> game = std::make_unique<Cricket>();
    game->play();
    game = std::make_unique<Football>();
    game->play();

    return 0;
}
