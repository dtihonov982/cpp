#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <array>
#include <cassert>
#include <algorithm>
#include "Definitions.h"

template <typename It>
void truncatePrint(It begin, It end, std::ostream& os, int width) {
    size_t i = 0;
    for (auto it = begin; it != end; ) {
        os << i << ":\t";
        for (size_t k = 0; it != end && k != width; ++k) {
            os << *it << '\t';
            ++it;
        }
        i += width;
        std::cout << '\n';
    }
}

class Memory {
public:
    Memory(const Block& data) {
        data_.fill(0);
        readStat_.fill(0);
        writeStat_.fill(0);
        assert(data.size() < MEM_SIZE);
        std::copy(data.begin(), data.end(), data_.begin());
    }

    Word get(Address address) {
        ++readStat_[address];
        return data_[address];
    }

    void set(Address address, Word word) {
        ++writeStat_[address];
        data_[address] = word;
    }

    size_t size() const {
        return data_.size();
    }

    void dump() {
        auto& os = std::cout;
        os << "Memory: \n";
        truncatePrint(data_.begin(), data_.end(), std::cout, OPCODE_WC);
    }

    void dumpReadings() {
        auto& os = std::cout;
        os << "Readings of memory: \n";
        truncatePrint(readStat_.begin(), readStat_.end(), std::cout, OPCODE_WC);        
    }

    void dumpWritings() {
        auto& os = std::cout;
        os << "Writing to memory: \n";
        truncatePrint(writeStat_.begin(), writeStat_.end(), std::cout, OPCODE_WC);        
    }
private:
    std::array<Word,   MEM_SIZE> data_;
    std::array<size_t, MEM_SIZE> readStat_;
    std::array<size_t, MEM_SIZE> writeStat_;
};

#endif
