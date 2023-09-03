//Reverse proxy for PostgreSQL 14.8 (Ubuntu 14.8-0ubuntu0.22.04.1) with query logging
//Build: g++ -o proxy main.cpp
//Possible usage: 
//1. Start this program on proxy server: proxy 127.0.0.1 5432 9999
//2. Start client: psql postgresql://localhost:9999/dvdrental?sslmode=disable

#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <string_view>
#include <memory>

#include "Exception.h"
#include "ProxyServer.h"

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "Usage: proxy <target ip> <target port> <proxy port>\n";
        return 0;
    }

    std::string_view ip = argv[1];
    int targetPort;
    try {
        targetPort = std::stoi(argv[2]);
    }
    catch (const std::invalid_argument& ex) {
        std::cerr << "Invalid target port " << argv[2] << std::endl;
        return 1;
    }
    int proxyPort;
    try {
        proxyPort = std::stoi(argv[3]);
    }
    catch (const std::invalid_argument& ex) {
        std::cerr << "Invalid proxy port " << argv[3] << std::endl;
        return 1;
    }

    try {
        std::string_view filename = "queries.log";
        auto logger = std::make_shared<QueryLogger>(filename);

        ProxyServer proxy(ip, targetPort, proxyPort, logger);
        proxy.run();
    }
    catch (const Exception& ex) {
        std::cerr << "Can not run proxy server. Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
