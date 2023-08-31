//Analysis which algorithm of getting extreme much faster.
//1. Simple https://github.com/ClickHouse/ClickHouse/blob/master/src/Columns/ColumnDecimal.cpp#LL399C26-L399C26
//2. Minimum compare Introduction to Algorithms, 3rd Edition  by Thomas H. Cormen...


#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <algorithm>
#include <chrono>
#include <cassert>
#include <sstream>
#include <variant>


//1. Simple https://github.com/ClickHouse/ClickHouse/blob/master/src/Columns/ColumnDecimal.cpp#LL399C26-L399C26
template <typename T>
void getExtreme_origin(const std::vector<T>& data, T& min, T& max) {
    if (data.empty()) {
        min = T{};
        max = T{};
        return;
    }

    min = data[0];
    max = data[0];
    

    for (const auto& x: data) {
        if (x < min)
            min = x;
        else if (max < x)
            max = x;
    }

    
    #if 0   
    auto it = data.cbegin();
    const auto end = data.cend(); 
    for (; it != end; ++it) {
        const auto it_v = *it;
        if (it_v < min)
            min = it_v;
        else if (it_v > max)
            max = it_v;
    } 
    #endif   
}

//2. Minimum compare Introduction to Algorithms, 3rd Edition  by Thomas H. Cormen...
template <typename T>
void getExtreme_fix(const std::vector<T>& data, T& min, T& max) {
    if (data.empty()) {
        min = T{};
        max = T{};
        return;
    }

    min = data[0];
    max = data[0];
    
    //if size not even start with second element
    auto first = data.begin() + (data.size() & 1);
    auto second = first + 1;
    auto end = data.end();
    while (first < end) {
        const auto first_v = *first;
        const auto second_v = *second;
 
        //getting pairs of elements and comparing them       
        if (first_v < second_v) {
            //then comparing with min and max
            if (first_v < min)
                min = first_v;
            if (max < second_v)
                max = second_v;
        }
        else {
            if (second_v < min)
                min = second_v;
            if (max < first_v)
                max = first_v;
        }
        
        first += 2;
        second = first + 1;
    }

}

std::vector<int> getRandVector(int size) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    std::vector<int> res;
    res.resize(size);
    for (int i = 0; i < size; ++i) {
        res.push_back(dist(rng));
    }
    return res;
}

//Like std::generate
template <typename GenType>
auto getRandVector(int size, GenType& gen) -> std::vector<decltype(gen())> {
    std::vector<decltype(gen())> result;
    result.reserve(size);
    for (int i = 0; i < size; ++i) {
        result.push_back(std::move(gen()));
    }
    return result;
}

//Generators for benchmarking

//Generator of random integer in [min; max]
class IntGen {
public:

    IntGen(int min, int max):
    rng{ std::random_device{}() },
    valueDist{min, max}
    { }

    int operator()() {
        return valueDist(rng);
    }
    
protected:
    std::mt19937 rng;
    std::uniform_int_distribution<int> valueDist;
};

//Generator of random string with fixed size or size in [minSize; maxSize]
class StrGen {
public:
    
    StrGen(int size): sizeGen{size}, randSize{false} {
    }
    
    StrGen(int minSize, int maxSize): sizeGen{IntGen{minSize, maxSize}} {
    }
    
    std::string operator()() {
        int cur_size = getSize();
        std::stringstream ss;
        for (int i = 0; i < cur_size; ++i) {
            ss << static_cast<char>(symGen());
        }
        return ss.str();
    }
    
    constexpr static int firstCode = 48;
    constexpr static int lastCode = 122;
    
private:

    int getSize() {
        if (randSize) 
            return std::get<IntGen>(sizeGen)();
        else 
            return std::get<int>(sizeGen);
    }
    
    IntGen symGen{firstCode, lastCode};
    std::variant<IntGen, int> sizeGen;
    const bool randSize = true;
};

//Tests

bool test_int(int size) {
    int min = -1;
    int max = -1;
    std::vector<int> vec = getRandVector(size);
    getExtreme_fix(vec, min, max);
    if (size > 0)
        return *std::min_element(vec.begin(), vec.end()) == min &&
               *std::max_element(vec.begin(), vec.end()) == max;
    else 
        return !min && !max;
}

bool test_str(int size) {
    std::string min = "-";
    std::string max = "-";
    
    auto gen = StrGen{10};
    std::vector<std::string> vec = getRandVector(5, gen);
    
    getExtreme_fix(vec, min, max);
    
    if (size > 0)
        return *std::min_element(vec.begin(), vec.end()) == min &&
               *std::max_element(vec.begin(), vec.end()) == max;
    else 
        return !min.empty() && !max.empty();
}

template <typename T>
using TargetFuncType = void (*)(const std::vector<T>&, T& , T&);

//Returns time of executing function Target with type Func and arguments args
template <typename Func, Func target, typename... Types>
std::chrono::milliseconds measure(Types&&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    target(args ...);
    auto stop = std::chrono::high_resolution_clock::now();    
    return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
}

//
template<typename GenType>
void bench(GenType& gen) {
    using ValueType = decltype(gen());
    for (int i = 0; i <= 5'000'000; i += 1'000'000) {
        auto vec = getRandVector(i, gen);
        
        ValueType min{};
        ValueType max{};
        auto origin = measure<TargetFuncType<ValueType>, getExtreme_origin>(vec, min, max).count();
        auto fix =  measure<TargetFuncType<ValueType>, getExtreme_fix>(vec, min, max).count();
        
        std::cout << i << '\t' << origin << '\t' << fix << std::endl;
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    for (const auto& x: vec) {
        os << x << ", ";
    }
    return os;
}

int main()
{
    std::cout << std::boolalpha << test_str(10) << std::endl;
    std::cout << std::boolalpha << test_int(10) << std::endl;

    auto gen = IntGen{-1000, 1000};
    bench(gen);
    
    return 0;
}
