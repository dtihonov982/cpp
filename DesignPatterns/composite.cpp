#include <iostream>
#include <memory>
#include <utility>
#include <string_view>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <sstream>

class Employee {
private:
    std::string name_;
    std::string dept_;
    int salary_;
    std::vector<Employee*> subordinates;
public:
    Employee(std::string_view name, std::string_view dept, int sal)
    : name_(name), dept_(dept), salary_(sal) {}

    void add(Employee& e) {
        subordinates.push_back(&e);
    }

    void remove(Employee& e)  {
        auto search = std::find(subordinates.begin(),
                subordinates.end(),
                &e);
        if (search != subordinates.end())
            subordinates.erase(search);
    }

    std::vector<Employee*> getSubordinates() {
        return subordinates;
    }

    std::string str() {
        std::stringstream ss;
        ss << name_ << " " << dept_ << " " << salary_;
        return ss.str();
    }
};

void recursivePrint(Employee& e, int depth = 0) {
    std::string tab(depth * 4, ' ');
    std::cout << tab << e.str() << "\n";
    for (auto s: e.getSubordinates()) {
        recursivePrint(*s, depth + 1);
    }
}

int main() {


      Employee CEO("John","CEO", 30000);

      Employee headSales("Robert","Head Sales", 20000);

      Employee headMarketing("Michel","Head Marketing", 20000);

      Employee clerk1("Laura","Marketing", 10000);
      Employee clerk2("Bob","Marketing", 10000);

      Employee salesExecutive1("Richard","Sales", 10000);
      Employee salesExecutive2("Rob","Sales", 10000);

      CEO.add(headSales);
      CEO.add(headMarketing);

      headSales.add(salesExecutive1);
      headSales.add(salesExecutive2);

      headMarketing.add(clerk1);
      headMarketing.add(clerk2);

      recursivePrint(CEO); 

      return 0;
}
