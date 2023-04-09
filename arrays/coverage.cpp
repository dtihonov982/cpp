#include <iostream>
#include <vector>

void insertion_sort(std::vector<int>& A) {
    if (A.empty())
        return;
    for (int j = 1; j < A.size(); ++j) { //n times 6
        int tmp = A[j]; //n-1 5
        int i = j - 1;  
        while (i >= 0 && A[i] > tmp) { //<= j + 1 in each iter, or 0.5*n*(n+1)-1 20
            A[i + 1] = A[i]; //<= j, 15
            --i;
        }
        A[i + 1] = tmp; //n - 1 times 5
    }
}

using iterator = std::vector<int>::iterator;
void merge(iterator lb, iterator rb, iterator re) {
    std::vector<int> L{lb, rb};
    iterator lit{L.begin()};
    std::vector<int> R{rb, re};
    iterator rit{R.begin()};
    iterator out{lb};
    while (out != re) { 
        if (lit != L.end() && (rit == R.end() || *lit <= *rit)) { 
            *out = *lit; 
            ++out; ++lit;
        }
        else {
            *out = *rit; 
            ++out; ++rit;
        }
    }
}

void mergeSort(iterator start, iterator end) {  //3
    int sz = std::distance(start, end);         //3
    if (sz > 1) {
        iterator middle = start + (sz) / 2;
        mergeSort(start, middle);
        mergeSort(middle, end);                 //1
        merge(start, middle, end);              //1
    }
}

int ceil(float x) {
    return x + 0.5f;
}

int floor(float x) {
    return x;
}

int MergeCall(int n) {
    if (n < 2) {
        return 1;
    }
    else {
        float m = n / 2.0;
        return 1 + MergeCall(floor(m)) + MergeCall(ceil(m));
    }
}

int main() {
    
    std::vector<int> A{10, 9, 8, 7, 6, 5, 4, 3, 2, 11};
    mergeSort(A.begin(), A.end());
    for (int x: A) std::cout << x << std::endl;
    return 0;
}
