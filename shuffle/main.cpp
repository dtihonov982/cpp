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
    for (const auto& e: vect) {
        if (result[e] != 0) {
            result[e]++;
        }
        else {
            result[e] = 1;
        }
    }
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

    std::vector<int> targetIndexes(vect.size());
    for (int i = 0; i < targetIndexes.size(); ++i) targetIndexes[i] = i;

    std::srand(std::time(NULL));
    for (const auto& e: vect) {
        assert(targetIndexes.size() > 0);

        int randKey = std::rand() % targetIndexes.size();
        int targetIndex = targetIndexes[randKey];
        result[targetIndex] = e;

        //todo: divide vector on 2 parts and swap elements between them.
        targetIndexes.erase(targetIndexes.begin() + randKey);
    }
    return result;
}



int main() {
    std::vector<int> a = {/*2, 0, 0, 3, 2, 0, 2, 3, 15, 40, 40, 1, 2, 3, 3*/};
    print(a);
    std::vector<int> randA = shuffle<int>(a);
    print(randA);
    std::cout << std::boolalpha << areEquivalent(a, randA) << std::endl;
    #if 0
    std::vector<int> b = {2, 1, 3};
    std::cout << std::boolalpha << areEquivalent<int>(a, b) << std::endl;
    auto m = countElem<int>(a);
    printMap<std::map<int, int>>(m);
    #endif

    return 0;
}
