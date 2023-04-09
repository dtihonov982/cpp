#include <iostream>
#include <vector>
#include "arrays.h"
#include <algorithm>
#include <iterator>
#include <chrono>



int main() {
    std::vector<int> A{5, 2, 4, 6, 1, 3};
    insertion_sort(A);
    return 0;
}

