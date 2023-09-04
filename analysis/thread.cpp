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

int main(int argc, char** argv) 
try {
    if (argc < 3) {
        std::cerr << "Usage: sum <data size> <threads count>\n";
        return 1;
    }
    int dataSize = std::stoi(argv[1]);
    int maxThreadsCount = std::stoi(argv[2]);
    auto data = getRandVector(dataSize, -1000000, 1000000);
    int64_t result = std::accumulate(data.begin(), data.end(), int64_t{});
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
catch(const std::invalid_argument& ex) {
    std::cerr << "Invalid argument of the program. ";
    std::cerr << ex.what() << std::endl;
    return 1;
}

std::vector<
    std::pair<It, It>
>
divide(It begin, It end, size_t count) {
    std::vector< std::pair<It, It> > result;
    int size = end - begin;
    int maxChunk = size % count ? size / count + 1 : size / count;

    for (int i = 0; i < count; ++i) {
        auto first = begin + i * maxChunk;
        auto last =  first + maxChunk;
        last = std::min(last, end);
        result.emplace_back(first, last);
    }

    return result;
}

int64_t sum(It begin, It end, size_t threadsCount) {
    int size = end - begin;
    if (size < 0)
        throw std::invalid_argument("data size < 0");
    if (size == 0)
        return 0;
    if (size < threadsCount)
        throw std::invalid_argument("data size < threads count");
    auto chunks = divide(begin, end, threadsCount);

    std::vector<int64_t> subresults(threadsCount);
    std::vector<std::thread> threads;
    threads.reserve(threadsCount); 
    size_t i = 0;
    for (auto [first, last]: chunks) {
        threads.emplace_back(&sumImpl, first, last, &subresults[i]);
        ++i;
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

