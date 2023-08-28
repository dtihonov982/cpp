#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdexcept>

class Client {
public:
    static Socket connectTo(std::string_view ip, int port) {

        Socket fd_ = socket(AF_INET, SOCK_STREAM, 0);

        if (fd_ < 0) 
            throw std::runtime_error("Socket creation error.");

        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        int status = inet_pton(AF_INET, ip.data() , &server_addr.sin_addr);

        if (status <= 0) 
            throw std::runtime_error("Invalid address.");

        status = connect(fd_, (sockaddr*)&server_addr, sizeof(server_addr));

        if (status < 0) 
            throw std::runtime_error("Connection failed.");

        return fd_;
    }
};

#endif // CLIENT_H
