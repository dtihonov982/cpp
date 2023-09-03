#include <stdexcept>
#include <iostream>
#include <memory>
#include <sys/socket.h>

#include "Exception.h"
#include "Server.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: server <port>\n";
        return 1;
    }
    int port;
    try {
        port = std::stoi(argv[1]);
    }
    catch (const std::invalid_argument& ex) {
        std::cerr << "Invalid port " << argv[1] << std::endl;
        return 1;
    }

    Server server(port);
    try {
        server.init();
    }
    catch (const std::runtime_error& ex) {
        std::cerr << "Can not run the server. Error: " << ex.what() << std::endl;
        return 1;
    }


    try {
        _Skt skt = server.accept();
        std::vector<char> buffer;
        buffer = skt.read();
        std::cout << buffer.data() << std::endl;
        skt.send(buffer);
    }
    catch (const std::runtime_error& ex) {
        std::cerr << "Can't send a data. " << ex.what() << std::endl;
        return 1;
    }


    return 0;
}
