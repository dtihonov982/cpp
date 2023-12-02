#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>
#include <sstream>

class Exception: public std::exception {
public:
    template<typename... Args>
    Exception(Args&&... args) {
        std::ostringstream oss;
        (oss << ... << args);
        message_ = oss.str();
    }

    const char* 
    what() const noexcept override { 
        return message_.c_str(); 
    }
private:
    std::string message_;
};

#endif //EXCEPTION_H
