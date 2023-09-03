#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdexcept>
#include <errno.h>
#include <string.h>
#include <vector>

#include "Socket.h"
#include "Acceptor.h"

std::ostream& log() {
    return std::cerr << "### ";
}

class Server {
public:
    Server(int port): acceptor_(port) {}

    void init() {
    }

    _Skt accept() {
        return acceptor_.accept();
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
    };
        
protected:
    Acceptor acceptor_;
};

#endif // SERVER_H
