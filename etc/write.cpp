#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>
#include <numeric>

#include <vector>
#include <iostream>
#include <chrono>
#include <string_view>
#include <cassert>

constexpr size_t Kb = 1024;
constexpr size_t Mb = 1024 * Kb;
constexpr size_t Gb = 1024 * Mb;

using Buffer = std::vector<char>;

Buffer getBuffer(size_t count) {
    return std::vector<char>(count, 'w');
}

using Benchmark = std::vector<int>;

Benchmark writeFile(std::string_view fileName, size_t fileSize, size_t bufferSize) {
    Benchmark result;
    int fd = creat(fileName.data(), 0);
    int operCount = fileSize / bufferSize;
    operCount += (fileSize % bufferSize != 0); //+1 if fileSize not divides on bufferSize
    for (int i = 0; i < operCount; ++i) {
        Buffer buff = getBuffer(bufferSize);
        auto start = std::chrono::high_resolution_clock::now();
        ssize_t written = write(fd, buff.data(), buff.size());
        auto stop = std::chrono::high_resolution_clock::now();    
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        result.push_back(duration);
        //std::cout << written << '\n';
        if  (written != bufferSize) {
            std::cout << "Error. written=" << written << '\n';
            break;
        }
    }
    close(fd);
    return result;
}

int main() {
    auto result = writeFile("test.dat", 10 * Mb, 128);
    std::cout << std::accumulate(result.begin(), result.end(), 0) << '\n';
    std::cout << result.size() << '\n';
   // close(res);

}
