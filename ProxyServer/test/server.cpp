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

    Socket skt;

    try {
        skt = server.accept();
    }
    catch (const std::runtime_error& ex) {
        std::cerr << "Error whiile accpeting a new connection. " << ex.what() << std::endl;
        return 1;
    }


    sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    getpeername(skt, (sockaddr*)&client_addr , &addrlen); 
    _Skt socket(skt, client_addr, addrlen);
    std::cout << socket.getAddress() << std::endl;
    std::cout << socket.getPort() << std::endl;


    std::vector<char> buffer;
    try {
        buffer = socket.read();
        std::cout << buffer.data() << std::endl;
    }
    catch (const std::runtime_error& ex) {
        std::cerr << "Can't read a data from the socket. " << ex.what() << std::endl;
        return 1;
    }

    try {
        socket.send(buffer);
    }
    catch (const std::runtime_error& ex) {
        std::cerr << "Can't send a data. " << ex.what() << std::endl;
        return 1;
    }


    return 0;
}
