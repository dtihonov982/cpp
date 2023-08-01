#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include <string_view>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>

using Socket = int;

std::ostream& log() {
    return std::cerr << "### ";
}

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

class ITrafficScanner {
public:
    virtual void scan(const std::vector<char>& data, Socket skt) = 0;
    virtual ~ITrafficScanner() {}
};

struct DbgScanner: public ITrafficScanner {
    static constexpr int width = 128;
    static bool printable(char c) { return c >= 33 && c <= 126; }

    void scan(const std::vector<char>& data, Socket skt) override {
        std::cout << '\n';
        for (int i = 0; i < data.size(); ) {
            for (int k = 0; i + k < data.size() && k < width; ++k) {
                if (printable(data[i + k])) std::cout << data[i + k];
                else std::cout << '.';
            }
            i += width;
            std::cout << '\n';
        }
    }
};

class QueryLogger: public ITrafficScanner {
public:
    QueryLogger(std::string_view filename): os_(filename.data(), std::ios::app) {}

    //Query message format: 1 byte - Message type, 4 bytes - Int32 message length, String - query text
    static constexpr int offset = 5;
    void scan(const std::vector<char>& data, Socket skt) override {
        if (data[0] != 'Q')
            return;
        os_ << "\n### Date: ";

        time_t now;
        time(&now);
        std::string_view time_str = ctime(&now);
        time_str.remove_suffix(1);
        os_ << time_str << " address: ";

        sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        getpeername(skt, (sockaddr*)&client_addr , &addrlen); 
        os_ << inet_ntoa(client_addr.sin_addr) 
              << ":" << ntohs(client_addr.sin_port) << " query: \n";

        os_.write(data.data() + offset, data.size() - offset);
        os_ << '\n';
        os_.flush();
    }
private:
    std::ofstream os_;
};

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
