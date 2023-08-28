#ifndef SELECTOR_H
#define SELECTOR_H

//#include <arpa/inet.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <sys/socket.h>
//#include <unistd.h>
//#include <errno.h>
//
//#include <string_view>
#include <set>
//#include <unordered_map>
#include <unordered_set>
//#include <vector>
//#include <stdexcept>
//#include <iostream>
//#include <fstream>

#include "Socket.h"

class Selector {
public:
    void insert(Socket skt) {
        poll_.insert(skt);
    }

    void erase(Socket skt) {
        poll_.erase(skt);
    }

    std::unordered_set<Socket> getSocketsForReading() {
        std::unordered_set<Socket> result;
        if (poll_.empty())
            return result;

        FD_ZERO(&fd_set_);
        for (auto skt: poll_)
            FD_SET(skt, &fd_set_);

        Socket maxFd = *(poll_.crbegin());

        int res = select(maxFd + 1, &fd_set_, NULL, NULL, NULL);

        if (res < 0 && errno != EINTR) {
            throw std::runtime_error(strerror(errno));
        }

        for (auto skt: poll_) {
            if (FD_ISSET(skt, &fd_set_))
                result.insert(skt);
        }

        return result;
    }

private:
    fd_set fd_set_;
    //ordered set for fast max element search
    std::set<Socket> poll_;
};

#endif // SELECTOR_H
