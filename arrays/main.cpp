#include <iostream>
#include <vector>
#include "arrays.h"
#include <algorithm>

int main() {
    int n = 100'000;
    int* A = new int[n];
    init_rand(A, n, 1000);
    //printd(A, n, false);
    //bubble_sort<int>(A, n);
    heapsort(A, n);
    //printd(A, n, false);

    #if 0
    std::vector<int> src = getRandVector(1000, 1000'000, true);
    //print<std::vector<int>>(src);

    std::vector<int> sorted{src};
    std::sort(sorted.begin(), sorted.end());
    //print<std::vector<int>>(sorted);
    std::cout << std::boolalpha << isSorted(src, sorted) << std::endl;
    #endif
    return 0;
}

