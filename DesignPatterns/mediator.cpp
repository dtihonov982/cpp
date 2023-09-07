#include <iostream>
#include <string>

class User;

class ChatRoom {
public:
    static void showMessage(User& user, const std::string& msg);
};

class User {
public:
    User(const std::string& name): name_(name) {}
    std::string getName() { return name_; }
    void sendMessage(const std::string& msg) {
        ChatRoom::showMessage(*this, msg);
    }
private:
    std::string name_;
};

void ChatRoom::showMessage(User& user, const std::string& msg) {
    std::cout << user.getName() << "> " << msg << std::endl;
}

int main() {
    User robert("Robert");
    User john("John");
    robert.sendMessage("Hi!");
    john.sendMessage("Hello!");
    return 0;
}
