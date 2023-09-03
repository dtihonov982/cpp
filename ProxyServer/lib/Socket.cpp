#include "Socket.h"
#include "Exception.h"
#include <sys/socket.h>
#include <arpa/inet.h>

std::vector<char> _Skt::read() {
    std::vector<char> data(1024);
    int count = ::read(fd_, data.data(), data.size());
    if (count < 0)
        throw Exception("Error while reading from socket. ", strerror(errno));
    data.resize(count);
    return data;
}

int _Skt::send(const std::vector<char>& data) {
    int count = ::send(fd_, data.data(), data.size(), 0);
    if (count != data.size()) 
        throw Exception("Error while sending a data to the socket. ", strerror(errno));
    return count;
}

std::string _Skt::getAddress() {
    return inet_ntoa(addr_.sin_addr);
}

uint16_t _Skt::getPort() {
    return ntohs(addr_.sin_port);
}

