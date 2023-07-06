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

Benchmark writeFile(int fd, size_t fileSize, size_t bufferSize) {
    Benchmark result;
    //int fd = creat(fileName.data(), 0);
    int operCount = fileSize / bufferSize;
    Buffer buff = getBuffer(bufferSize);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < operCount; ++i) {
        //auto start = std::chrono::high_resolution_clock::now();
        ssize_t written = write(fd, buff.data(), buff.size());
#if 0
        auto stop = std::chrono::high_resolution_clock::now();    
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        result.push_back(duration);
        std::cout << written << '\n';
        if  (written != bufferSize) {
            std::cout << "Error. written=" << written << '\n';
            break;
        }
#endif
    }
    auto stop = std::chrono::high_resolution_clock::now();    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << bufferSize << ';' << duration << '\n';
    //close(fd);
    return result;
}

int main() {
    int fd = creat("test.dat", 0);
    for (int i = 32; i < 8*Kb; i <<= 1)
    //for (int i = 512; i <= 16*Kb; i += 512)
        writeFile(fd, 10 * Mb, i);
    close(fd);
    //std::cout << std::accumulate(result.begin(), result.end(), 0) << '\n';
    //std::cout << result.size() << '\n';
   // close(res);

}
