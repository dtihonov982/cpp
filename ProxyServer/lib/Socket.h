#ifndef SOCKET_H
#define SOCKET_H

#include <unistd.h>
#include <stdio.h>
#include <stdexcept>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>

using Socket = int;

class Connection {
public:
    template<typename T>
    static int send(Socket skt, const T& data) {
        int count = ::send(skt, data.data(), data.size(), 0);
        if (count != data.size()) 
            throw std::runtime_error(strerror(errno));
        return count;
    }

    static std::vector<char> read(Socket skt) {
        std::vector<char> data(1024);
        int count = ::read(skt, data.data(), data.size());
        data.resize(count);
        return data;
    }
};

class _Skt {
public:
    _Skt(int fd, const sockaddr_in& addr, socklen_t addrlen)
    : fd_(fd), addr_(addr), addrlen_(addrlen) {}

    std::string getAddress() {
        return inet_ntoa(addr_.sin_addr);
    }

    uint16_t getPort() {
        return ntohs(addr_.sin_port);
    }
    
    _Skt(const _Skt&) = delete;
    _Skt& operator=(const _Skt&) = delete;
    _Skt(_Skt&&) = default;
    _Skt& operator=(_Skt&&) = default;

    ~_Skt() { close(fd_); }

    std::vector<char> read() {
        std::vector<char> data(1024);
        int count = ::read(fd_, data.data(), data.size());
        data.resize(count);
        return data;
    }

    template<typename T>
    int send(const T& data) {
        int count = ::send(fd_, data.data(), data.size(), 0);
        if (count != data.size()) 
            throw std::runtime_error(strerror(errno));
        return count;
    }

private:
    int fd_;
    sockaddr_in addr_;
    socklen_t addrlen_;
};

#endif // SOCKET_H
