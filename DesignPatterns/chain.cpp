#include <iostream>
#include <string_view>
#include <vector>
#include <memory>

#define INFO   1
#define DEBUG  2
#define ERROR  3

class AbstractLogger {
public:

    void setNextLogger(AbstractLogger* nextLogger) {
        nextLogger_ = nextLogger;
    }

    void logMessage(int level, std::string_view msg) {
        if (level_ <= level) {
            write(msg);
        }
        if (nextLogger_) {
            nextLogger_->logMessage(level, msg);
        }

    }

protected:
    AbstractLogger(int level): level_(level) {}
    virtual void write(std::string_view msg) = 0;

    int level_;
    AbstractLogger* nextLogger_ = nullptr;
};

class ConsoleLogger: public AbstractLogger {
public:
    ConsoleLogger(int level): AbstractLogger(level) {}
protected:
    void write(std::string_view msg) override {
        std::cout << "Standard Console::Logger: " << msg << '\n';
    }
};

class ErrorLogger: public AbstractLogger {
public:
    ErrorLogger(int level): AbstractLogger(level) {}
protected:
    void write(std::string_view msg) override {
        std::cout << "Error Console::Logger: " << msg << '\n';
    }
};

class FileLogger  : public AbstractLogger {
public:
    FileLogger (int level): AbstractLogger(level) {}
protected:
    void write(std::string_view msg) override {
        std::cout << "File Console::Logger: " << msg << '\n';
    }
};

int main() {
    std::shared_ptr<AbstractLogger> errorLogger (new ErrorLogger(ERROR));
    std::shared_ptr<AbstractLogger> fileLogger (new FileLogger(DEBUG));
    std::shared_ptr<AbstractLogger> consoleLogger (new ConsoleLogger(INFO));

    errorLogger->setNextLogger(fileLogger.get());
    fileLogger->setNextLogger(consoleLogger.get());

    auto loggerChain = errorLogger;

    loggerChain->logMessage(INFO, "regular information");
    loggerChain->logMessage(DEBUG, "debug information");
    loggerChain->logMessage(ERROR, "error information");

    return 0;
}