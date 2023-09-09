#include <iostream>
#include <vector>
#include <memory>

class NameRepo {
private:
    class NameIterator {
    public:
        NameIterator(NameRepo& repo): repo_(repo) {}
        bool hasNext() {
            if (index_ < repo_.names.size())
                return true;
            else
                return false;
        }
        std::string next() {
            if (hasNext())
                return repo_.names[index_++];
            else
                return {};
        }
    private:
        NameRepo& repo_;
        size_t index_ = 0;
    };
public:
    std::vector<std::string> names = {"Robert", "John", "Julie", "Lora"};
    NameIterator getIterator() {
        return NameIterator(*this);
    }
};

int main() {
    NameRepo repo;
    for (auto it = repo.getIterator(); it.hasNext();) {
        auto name = it.next();
        std::cout << name << std::endl;
    }

    return 0;
}
