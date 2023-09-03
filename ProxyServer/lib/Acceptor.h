#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdexcept>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Socket.h"

class Acceptor {
public:
    Acceptor(uint16_t port);

    std::string getAddress();
    uint16_t getPort();
    
    Acceptor(const Acceptor&) = delete;
    Acceptor& operator=(const Acceptor&) = delete;
    Acceptor(Acceptor&&) = default;
    Acceptor& operator=(Acceptor&&) = default;

    ~Acceptor() { shutdown(fd_, SHUT_RDWR); }

    _Skt accept();
private:
    int fd_;
    sockaddr_in addr_;
    socklen_t addrlen_;
};


