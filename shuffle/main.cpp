#include <iostream>
#include <vector>
//#include <set>
#include <map>
#include <cassert>
#include <cstdlib>
#include <ctime>


template<typename T>
void print(const T& container) {
    for (const auto& e: container) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}

template<typename M>
void printMap(const M& map) {
    for (const auto& [k, v]: map) {
        std::cout << k << ": " << v << " ";
    }
    std::cout << std::endl;
}

template<typename T>
std::map<T, int> countElem(const std::vector<T>& vect) {
    std::map<T, int> result;
    for (const auto& e: vect) result[e]++;
    return result;
}
        

template<typename T>
bool areEquivalent(const std::vector<T>& a, const std::vector<T>& b) {
    auto mapA = countElem(a);
    auto mapB = countElem(b);
    return mapA == mapB;
}

template<typename T>
std::vector<T> shuffle(const std::vector<T>& vect) {
    std::vector<T> result(vect.size());

    int sizeOfIndArr = vect.size();
    std::vector<int> targetIndexes(sizeOfIndArr);
    for (int i = 0; i < sizeOfIndArr; ++i) targetIndexes[i] = i;
    
    std::srand(std::time(NULL));
    for (const auto& e: vect) {
        assert(sizeOfIndArr > 0);

        int randKey = std::rand() % sizeOfIndArr;
        int targetIndex = targetIndexes[randKey];
        result[targetIndex] = e;

        //Target indexes array divides on 2 parts. Left hold not picked indexes, right holds picked indexes.
        std::swap(targetIndexes[randKey], targetIndexes[--sizeOfIndArr]);
    }
    return result;
}

std::vector<int> getRandVector(int maxValue, int maxSize, bool fixedSize = false) {
    std::srand(std::time(NULL));
    int size;
    if (fixedSize) {
        size = maxSize;
    }
    else {
        size  = std::rand() % (maxSize + 1);
    }
    std::vector<int> res(size);
    for (int i = 0; i < size; ++i) {
        res[i] = std::rand() % (maxValue + 1);
    }
    return res;
}

int main() {
    std::vector<int> a = getRandVector(30, 30, true);
    print<std::vector<int>>(a);
    std::vector<int> randA = shuffle<int>(a);
    print<std::vector<int>>(randA);
    std::cout << std::boolalpha << areEquivalent(a, randA) << std::endl;
    #if 0
    std::vector<int> a = {2, 0, 0, 3, 2, 0, 2, 3, 15, 40, 40, 1, 2, 3, 3};
    std::vector<int> b = {2, 1, 3};
    std::cout << std::boolalpha << areEquivalent<int>(a, b) << std::endl;
    auto m = countElem<int>(a);
    printMap<std::map<int, int>>(countElem<int>(a));
    #endif

    return 0;
}
