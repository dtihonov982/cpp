//Analysis how new operator allocates memory in heap. 

#include <iostream>
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>

using DataType = char;

//Information about allocated memory
struct Block {
    DataType* data;
    int dataSize;
    
    static constexpr int sysInfoSize = 8;
    
    DataType* realBegin() const { return data - sysInfoSize; }
    void* vrealBegin() const { return static_cast<void*>(realBegin()); }
    
    static int distanceBetweenBegins(const Block first, const Block second) { 
        return second.realBegin() - first.realBegin(); 
    } 
    
    static int distanceToNext(const Block first, const Block second) { 
        return second.realBegin() - (first.realBegin() + first.realSize()); 
    } 
    
    int realSize() const { return dataSize + sysInfoSize; }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& cont) {
    for (const auto& x: cont) {
        os << x << ' ';
    }
    return os;
}

class TableFormatter {
public:
    TableFormatter(std::initializer_list<std::string> colNames)
    : colNames_(colNames)
    , colCount_(colNames_.size())
    {  
    }
    
    void print() const { std::cout << '\n'; }
    
    template <typename Head, typename... Tail>
    void print(const Head& head, const Tail&... tail) const {
        std::cout << head << '\t';
        print(tail...);
    }
    
    void printTitle() const {
        for (const auto& title: colNames_) {
            std::cout << title << '\t';
        }
        std::cout << '\n';
    }
private:
    std::vector<std::string> colNames_;
    int colCount_;
};

int main()
{
    constexpr int blocksCount = 256;
    constexpr int blockSize = 24;
    
    std::vector<int> sizes(blocksCount, blockSize);

    //allocation
    std::vector<Block> bls;
    for (auto size: sizes) {
       Block blk{new DataType[size], size};
       bls.push_back(blk);
    }
    
    TableFormatter tf{"Address", "num" "d(prev,curr)", "Size", "Total", "relativeAdress"};
    tf.printTitle();

    //first block
    auto prev = bls[0];
    int allocated = prev.realSize();
    int relativeAdress = 0;
    tf.print(prev.vrealBegin(), 1, 0,  prev.realSize(), allocated, relativeAdress);
    
    for (int i = 1; i < blocksCount; ++i) {
        auto curr = bls[i];
        allocated += curr.realSize();
        relativeAdress = curr.realBegin() - bls[0].realBegin();

        tf.print(curr.vrealBegin()
                , i + 1
                , Block::distanceToNext(prev, curr)
                , curr.realSize()
                , allocated, relativeAdress);

        prev = curr;
    }
    
    for (auto& blk: bls) {
        delete[] blk.data;
    }

    return 0;
}
