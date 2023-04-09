#include <iostream>
#include <vector>

class Stack {
    std::vector<char> data;
    int sz;
public:
    Stack(int capacity): data(capacity, '_'), sz(0) {}
    void push(char x) { data[sz++] = x; }
    void pop() { --sz; }
    std::string to_str() { return std::string{data.begin(), data.end()}; }
    int size() { return sz; }
    bool not_empty() { return sz > 0; }
};

        


int main() {
    
    int n = 2;
    Stack stack(n);
    
    char curr = '(';
    stack.push(curr);
    int control = 1;
    while (stack.not_empty()) {
        while (stack.size() <= n && n - stack.size() >= control) {
            stack.push(curr);
            curr = '(';
            ++control;
        }
        //todo: change control var
        //curr = stack.top();
        stack.pop();

        if (control == 0 && stack.size() == n) {
            std::cout << stack.to_str() << std::endl;
        }

        if (control >= 0) {
            curr = ')';
            --control;
        }
    }
    return 0;
}




