#pragma once 

#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <memory>
#include <string_view>

namespace token {

struct Token {
    enum type { LABEL, COMMAND};
    Token(type t): what(t) {}
    type what;

    virtual void print() const = 0;
};

using TokenPtr = std::shared_ptr<Token>;

class Command: public Token {
public:
    Command(const std::string& name, const std::vector<std::string>& args)
    : Token(Token::COMMAND), name_(name), args_(args) {
    }
    void print() const override {
        std::cout << "COMMAND " << name_ << " ";
        for (auto& a: args_)
            std::cout << a << " ";
        std::cout << std::endl;
    }
    std::string getName() const { return name_; }
    std::vector<std::string> getArgs() const { return args_; }
private:
    std::string name_;
    std::vector<std::string> args_;
};

using CommandPtr = std::shared_ptr<Command>;

class Label: public Token {
public:
    Label(const std::string& name)
    : Token(Token::LABEL)
    , name_(name) {
    }
    void print() const override {
        std::cout << "LABEL " << name_ << std::endl;
    }
    std::string getName() const { return name_; }
private:
    std::string name_;
};

bool isLabel(const std::string& line);

std::string extractLabelFromDefinition(const std::string& line);

std::string strip(const std::string& s);

std::vector<std::string> split(std::string_view input, char delim);

std::vector<TokenPtr> tokenize(std::istream& is);

} // namespace token
