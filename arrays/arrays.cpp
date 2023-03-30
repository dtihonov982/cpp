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

