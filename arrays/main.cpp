#include <iostream>
#include <vector>
#include "arrays.h"
#include <algorithm>
#include <iterator>
#include <chrono>



int main() {

    std::vector<int> a{5, 4, 3, 1, 2};
    print<std::vector<int>>(a);
    //int q = partition<int>(a, 0, a.size() - 1);
    //print<std::vector<int>>(a);
    //std::cout << q << ": " << a[q] << std::endl;
    int rang;
    std::cin >> rang;
    std::cout << getByRang(a, rang) << std::endl;
    
    std::vector<int> src{getRandVector(1000, 400'000, RandGenerationMode::fixedSize)};
    #if 0
    int n = 400'000;
    int* A = new int[n];
    init_rand(A, n, 1000);
    std::vector<int> src{getRandVector(1000, 400'000, true)};
    //print<std::vector<int>>(src);
    std::vector<int> sorted{src};
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    heapsort(A, n);
    //quicksort<int>(sorted);
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    //print<std::vector<int>>(sorted);
    std::chrono::duration<double> diff = end - start;
    std::cout << std::boolalpha << isSorted(src, sorted) << std::endl;
    std::cout << diff.count() << std::endl;
    print<std::vector<int>>(A);
    int i = partition<int>(A, 0, A.size() - 1);
    print<std::vector<int>>(A);
    std::cout << i << ": " << A[i] << std::endl;

    int n = 100'000;
    int* A = new int[n];
    init_rand(A, n, 1000);
    //printd(A, n, false);
    //bubble_sort<int>(A, n);
    heapsort(A, n);
    //printd(A, n, false);

    std::vector<int> src = getRandVector(1000, 1000'000, true);
    //print<std::vector<int>>(src);

    std::vector<int> sorted{src};
    std::sort(sorted.begin(), sorted.end());
    //print<std::vector<int>>(sorted);
    std::cout << std::boolalpha << isSorted(src, sorted) << std::endl;
    #endif
    return 0;
}

