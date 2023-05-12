#include "arrays.h"

void heapsort(int A[], int length) {
    build_max_heap(A, length);
    int heap_size = length;
    for (int i = length-1; i > 1; i--) {
        swap_elements(A[0], A[i]);
        heap_size = heap_size - 1;
        max_heapify(A, heap_size, 0);
    }
}

void build_max_heap(int A[], int length) {
    int heap_size = length;
    for (int i = length/2-1; i >= 0; i--) {
        max_heapify(A, heap_size, i);
    }
}

void max_heapify(int A[], int heap_size, int i) {
    int l = left(i);
    int r = right(i);
    int largest;
    if(l <= (heap_size - 1) && A[l] > A[i])
        largest = l;
    else
        largest = i;
    if (r <= (heap_size - 1) && A[r] > A[largest])
        largest = r;
    if (largest != i) {
        swap_elements(A[i], A[largest]);
        max_heapify(A, heap_size, largest);
    }
}

int parent(int i){
    if(!i) return 0;
    return (i-1)/2;
}

int left(int i) {
    return 2*i+1;
}

int right(int i) {
        return 2*(i + 1);
}


void init_rand(int A[], const int length, int maxValue) {
    std::random_device rd;     // Only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(1, maxValue); // Guaranteed unbiased

    for(int i = 0; i < length; i++) {
        A[i] = uni(rng);
    }
}

//Fills an array by descending integer numbers.
void init_descending(int A[], const int length) {
    for(int i = 0; i < length; i++) {
        A[i] = length - i;
    }
}

void swap_elements(int& a, int& b) {
    int x = a;
    a = b;
    b = x;
}

bool is_equal(int A[], int B[], int length) {
    for (int i = 0; i < length; i++) {
        if (A[i] != B[i]) return false;
    }
    return true;
}

void insertion_sort(std::vector<int>& A) {
    if (A.empty())
        return;
    for (int j = 1; j < A.size(); ++j) {
        int tmp = A[j];
        int i = j - 1;
        while (i >= 0 && A[i] > tmp) {
            A[i + 1] = A[i];
            --i;
        }
        A[i + 1] = tmp;
    }
}
std::vector<int> getRandVector(int maxValue, int maxSize, RandGenerationMode mode) {
    std::srand(std::time(NULL));
    int size;
    if (mode == RandGenerationMode::fixedSize) {
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
