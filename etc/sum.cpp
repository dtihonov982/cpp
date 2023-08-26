#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <chrono>
#include <random>
#include <cassert>
#include <stdexcept>
#include <cstdint>

using It = decltype(std::vector<int>{}.cbegin());
int64_t sum(It begin, It end, size_t threadsCount = 1);
void sumImpl(It begin, It end, int64_t* result);
std::vector<int> getRandVector(int size, int min, int max);
class Table {
public:
    Table(const std::string& title): title_(title) {}
    void add(int a, int b);
    void print();
private:
    std::vector<std::pair<int, int>> rows_;
    std::string title_;
};

int main(int argc, char** argv) {
    if (argc < 3) 
        return 1;
    int dataSize = std::stoi(argv[1]);
    auto data = getRandVector(dataSize, -1000000, 1000000);
    int64_t result = std::accumulate(data.begin(), data.end(), int64_t{});
    int maxThreadsCount = std::stoi(argv[2]);
    Table table("Count\tMs");
    for (int i = maxThreadsCount; i > 0; --i) {
        auto start = std::chrono::high_resolution_clock::now();

        int64_t currResult = sum(data.cbegin(), data.cend(), i);

        auto stop = std::chrono::high_resolution_clock::now();    
        assert(currResult == result);
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        table.add(i, duration.count());
    }
    table.print();
    return 0;
}

int64_t sum(It begin, It end, size_t threadsCount) {
    int size = end - begin;
    if (size < 0)
        throw std::invalid_argument("begin < end");
    if (size == 0)
        return 0;
    if (size < threadsCount)
        throw std::invalid_argument("size < threads count");
    int maxChunk = size % threadsCount ? size / threadsCount + 1 : size / threadsCount;
    std::vector<int64_t> subresults(threadsCount);
    std::vector<std::thread> threads;
    threads.reserve(threadsCount); 
    for (int i = 0; i < threadsCount; ++i) {
        auto first = begin + i * maxChunk;
        auto last =  first + maxChunk;
        last = std::min(last, end);
        threads.emplace_back(&sumImpl, first, last, &subresults[i]);
    }

    int64_t total = 0;
    for (int i = 0; i < threadsCount; ++i) {
        threads[i].join();
        total += subresults[i];
    }
    return total;
}

void sumImpl(It begin, It end, int64_t* result) {
    int64_t tmp = 0;
    for (; begin < end; ++begin) {
        tmp += *begin;
    }
    *result = tmp;
}

std::vector<int> getRandVector(int size, int min, int max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

    std::vector<int> res;
    res.reserve(size);
    for (int i = 0; i < size; ++i) {
        res.push_back(dist(rng));
    }
    return res;
}

void Table::add(int a, int b) {
    rows_.emplace_back(a, b);
}

void Table::print() {
    std::cout << title_ << std::endl;
    for (auto& row: rows_) {
        std::cout << row.first << "\t" << row.second << "\n";
    }
}

