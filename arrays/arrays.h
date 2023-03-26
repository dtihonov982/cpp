#ifndef ARRAYS_H
#define ARRAYS_H

#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <cassert>
#include <cstdlib>
#include <ctime>

void heapsort(int A[], int length);
void build_max_heap(int A[], int length);
void max_heapify(int A[], int heap_size, int i);
int parent(int i);
int left(int i);
int right(int i);
void init_rand(int A[], const int length, int maxValue);
void swap_elements(int& a, int& b);
void init_descending(int A[], const int length);
bool is_equal(int A[], int B[], int length);
std::vector<int> getRandVector(int maxValue, int maxSize, bool fixedSize = false);

template <typename T>
void bubble_sort(T A[], int length) {
    if (length < 2) return;
    T x;
    for (int i = 1; i < length; i++) {
        for (int j = i; j > 0; j--) {
            if (A[j] < A[j-1]) {
                x = A[j-1];
                A[j-1] = A[j];
                A[j] = x;

            } else {
                break;
            }
        }
    }

}

//Print array to console. If prefix true also prints indexes of elements.
template <typename T>
void printd(T A[], int length, bool prefix=true) {
    for (int i = 0; i < length; i++) {
        if (prefix) std::cout << i << ":";
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;
}

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


template<typename T>
bool isSorted(std::vector<T> src, std::vector<T> sorted) {
    //Check does sorted has order
    for (auto it = sorted.begin(); it != sorted.end(); ++it) {
        if (*(it - 1) > *it)
            return false;
    }
    //Check does sorted has same element as src
    return areEquivalent(src, sorted);
}

#endif
