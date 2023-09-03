#include "Acceptor.h"

Acceptor::Acceptor(uint16_t port) {
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = INADDR_ANY;
    addr_.sin_port = htons(port);

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

    //log() << "Listening port " << port << '\n';
}

_Skt Acceptor::accept() {
    sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int new_socket = ::accept(fd_, (sockaddr*) &client_addr, &addrlen);
    if (new_socket < 0) {
        throw std::runtime_error(strerror(errno));
    }
    
    return _Skt(new_socket, client_addr, addrlen);
}
