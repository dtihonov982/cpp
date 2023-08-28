#ifndef PROXYSERVER_H
#define PROXYSERVER_H

//#include <arpa/inet.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/socket.h>
//#include <unistd.h>
//#include <errno.h>
//
#include <string_view>
//#include <set>
#include <unordered_map>
//#include <unordered_set>
#include <vector>
//#include <stdexcept>
#include <iostream>
//#include <fstream>
//
#include "Socket.h"
#include "Client.h"
#include "Scanner.h"
#include "Server.h"
#include "Selector.h"

class ProxyServer: public Server {
public:
 
    ProxyServer(std::string_view targetIp, int targetPort, int proxyPort, ITrafficScanner* scanner)
    : Server(proxyPort), targetIp_(targetIp), targetPort_(targetPort), scanner_(scanner) {
    }   

    ProxyServer(std::string_view targetIp, int targetPort, int proxyPort)
    :  ProxyServer(targetIp, targetPort, proxyPort, nullptr) {
    }

    void openPipe(Socket connWithClient) {
        selector_.insert(connWithClient);

        Socket connWithTarget = Client::connectTo(targetIp_, targetPort_);
        selector_.insert(connWithTarget);

        pairs_[connWithTarget] = connWithClient;
        pairs_[connWithClient] = connWithTarget;
    }

    void closePipe(Socket first, Socket second) {
        closeConnection(first);
        selector_.erase(first);
        pairs_.erase(first);

        closeConnection(second);
        selector_.erase(second);
        pairs_.erase(second);
    }

    void run() {
        init();
        selector_.insert(fd_);
        while (true) {

            auto readingPoll = selector_.getSocketsForReading();
            if (readingPoll.empty())
                break;

            if (readingPoll.count(fd_) > 0) {
                Socket connWithClient = accept();
                openPipe(connWithClient);
                readingPoll.erase(fd_);
            }

            for (auto conn: readingPoll) {
                std::vector<char> buffer = Connection::read(conn);

                if (scanner_)
                    scanner_->scan(buffer, conn);

                Socket relatedConn = pairs_[conn];

                if (buffer.empty()) {
                    closePipe(conn, relatedConn);
                }
                else {
                    Connection::send(relatedConn , buffer);
                }
            }
        }
    }

private:
    std::string targetIp_;
    int targetPort_;
    std::unordered_map<Socket, Socket> pairs_;
    Selector selector_;
    ITrafficScanner* scanner_;
};
#endif // PROXYSERVER_H
