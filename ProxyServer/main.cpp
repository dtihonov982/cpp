#include "Proxy.h"
#include <fstream>
#include <cstdlib>
#include <string_view>

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "Usage: proxy <target ip> <target port> <proxy port>\n";
        return 0;
    }

    std::string_view ip = argv[1];
    int targetPort = std::atoi(argv[2]);
    int proxyPort = std::atoi(argv[3]);

    if (!targetPort || !proxyPort || ip.empty()) {
        std::cout << "Invalid arguments.\n";
        return 0;
    }

    std::string_view filename = "queries.log";
    QueryLogger scanner(filename);

    ProxyServer proxy(ip, targetPort, proxyPort, &scanner);
    proxy.run();
    return 0;
}
