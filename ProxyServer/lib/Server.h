#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdexcept>
#include <errno.h>
#include <string.h>
#include <vector>

#include "Socket.h"

std::ostream& log() {
    return std::cerr << "### ";
}

class Server {
public:
    Server(int port): port_(port) {
        addr_.sin_family = AF_INET;
        addr_.sin_addr.s_addr = INADDR_ANY;
        addr_.sin_port = htons(port_);
    }

    void init() {
        fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if ( fd_ < 0) {
            throw std::runtime_error(strerror(errno));
        }

        int opt = 1;
        if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            throw std::runtime_error(strerror(errno));
        }

        if (bind(fd_, (sockaddr*) &addr_, sizeof(addr_)) < 0) {
            throw std::runtime_error(strerror(errno));
        }

        if (listen(fd_, 3) < 0) {
            throw std::runtime_error(strerror(errno));
        }
    
        log() << "Listening port " << port_ << '\n';
    }

    Socket accept() {
        sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int new_socket = ::accept(fd_, (sockaddr*) &client_addr, &addrlen);
        if (new_socket < 0) {
            throw std::runtime_error(strerror(errno));
        }

        log() << "New connection. Socket fd: " << new_socket 
              << ", ip is " << inet_ntoa(client_addr.sin_addr) 
              << ", port: " << ntohs(client_addr.sin_port) << '\n';

        return new_socket;
    }

    void closeConnection(Socket client_fd) {
        sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        getpeername(client_fd, (sockaddr*)&client_addr , &addrlen); 
        log() << "Connection closed. Socket fd: " << client_fd
              << ", ip is " << inet_ntoa(client_addr.sin_addr) 
              << ", port: " << ntohs(client_addr.sin_port) << '\n';
        close(client_fd);
        
    }
    virtual ~Server() {
        shutdown(fd_, SHUT_RDWR);
    };

    int getMasterSocket() {
        return fd_;
    }
        
protected:
    int port_;
    Socket fd_ = -1;
    sockaddr_in addr_;
};

#endif // SERVER_H
