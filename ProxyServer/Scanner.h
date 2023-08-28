#ifndef SCANNER_H
#define SCANNER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string_view>

#include "Socket.h"

class ITrafficScanner {
public:
    virtual void scan(const std::vector<char>& data, Socket skt) = 0;
    virtual ~ITrafficScanner() {}
};

struct DbgScanner: public ITrafficScanner {
    static constexpr int width = 128;
    static bool printable(char c) { return c >= 33 && c <= 126; }

    void scan(const std::vector<char>& data, Socket skt) override {
        std::cout << '\n';
        for (int i = 0; i < data.size(); ) {
            for (int k = 0; i + k < data.size() && k < width; ++k) {
                if (printable(data[i + k])) std::cout << data[i + k];
                else std::cout << '.';
            }
            i += width;
            std::cout << '\n';
        }
    }
};

class QueryLogger: public ITrafficScanner {
public:
    QueryLogger(std::string_view filename): os_(filename.data(), std::ios::app) {}

    //Query message format: 1 byte - Message type, 4 bytes - Int32 message length, String - query text
    static constexpr int offset = 5;
    void scan(const std::vector<char>& data, Socket skt) override {
        if (data[0] != 'Q')
            return;
        os_ << "\n### Date: ";

        time_t now;
        time(&now);
        std::string_view time_str = ctime(&now);
        time_str.remove_suffix(1);
        os_ << time_str << " address: ";

        sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        getpeername(skt, (sockaddr*)&client_addr , &addrlen); 
        os_ << inet_ntoa(client_addr.sin_addr) 
              << ":" << ntohs(client_addr.sin_port) << " query: \n";

        os_.write(data.data() + offset, data.size() - offset);
        os_ << '\n';
        os_.flush();
    }
private:
    std::ofstream os_;
};

#endif // SCANNER_H
