#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <algorithm>
#include <chrono>
#include <cassert>
#include <sstream>
#include <variant>

std::vector<int> getRandVector(int size) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    std::vector<int> res;
    res.resize(size);
    for (int i = 0; i < size; ++i) {
        res.push_back(dist(rng));
    }
    return res;
}

template <typename GenType>
auto getRandVector(int size, GenType& gen) -> std::vector<decltype(gen())> {
    std::vector<decltype(gen())> result;
    result.reserve(size);
    for (int i = 0; i < size; ++i) {
        result.push_back(std::move(gen()));
    }
    return result;
}

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
    std::uniform_int_distribution<std::mt19937::result_type> valueDist;
};

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

std::string getRandStr(int size) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(33, 126); 
    
    std::stringstream ss;
    for (int i = 0; i < size; ++i) {
        ss << static_cast<char>(dist(rng));
    }
    
    return ss.str();
}    
    
template <typename T>
void getExtreme_origin(const std::vector<T>& data, T& min, T& max) {
    if (data.empty()) {
        min = T{};
        max = T{};
        return;
    }

    min = data[0];
    max = data[0];
    
    #if 0
    for (const auto& x: data) {
        if (x < min)
            min = x;
        else if (max < x)
            max = x;
    }
    #endif
    
    auto it = data.cbegin();
    const auto end = data.cend(); 
    for (; it != end; ++it) {
        const auto it_v = *it;
        if (it_v < min)
            min = it_v;
        else if (it_v > max)
            max = it_v;
    }    
}

template <typename T>
void getExtreme_fix(const std::vector<T>& data, T& min, T& max) {
    if (data.empty()) {
        min = T{};
        max = T{};
        return;
    }

    min = data[0];
    max = data[0];
    
    #if 1
    auto first = data.begin() + (data.size() & 1);
    auto second = first + 1;
    auto end = data.end();
    while (first < end) {
        const auto first_v = *first;
        const auto second_v = *second;
        
        if (first_v < second_v) {
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
    #endif
    

}

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

template <typename Func, Func func>
std::chrono::milliseconds measure_(int size) {
    std::vector<int> vec = getRandVector(size);
    int min = -1;
    int max = -1;    
    
    auto start = std::chrono::high_resolution_clock::now();
    func(vec, min, max);
    auto finish = std::chrono::high_resolution_clock::now();
    
    if (size > 0)
        assert( *std::min_element(vec.begin(), vec.end()) == min &&
               *std::max_element(vec.begin(), vec.end()) == max );
    else 
        assert( !min && !max ); 
    return std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
}

template <typename Func, Func target, typename... Types>
std::chrono::milliseconds measure(Types&&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    target(args ...);
    auto stop = std::chrono::high_resolution_clock::now();    
    return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
}

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

void test_all() {
    bool passed = false;
    for (int i = 0; i < 1'000; ++i) {
        passed = test_int(i);
        if (!passed)
            break;
    }   
    std::cout << std::boolalpha << passed << std::endl;
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
    
    std::cout << IntGen{-1000, 1000}() << std::endl;
    
    return 0;
}
