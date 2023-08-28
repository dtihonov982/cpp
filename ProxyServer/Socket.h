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

#endif // SOCKET_H
