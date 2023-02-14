#include <iostream>
#include <random>
#include <chrono>

using namespace std;

//Fills an array by random int numbers.
void init_rand(int A[], const int length) {
    std::random_device rd;     // Only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(1, 1000); // Guaranteed unbiased

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


void int_bubble_sort(int A[], int length) {
    if (length < 2) return;
    int x;
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

//An ascending sorting of an array.
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
        if (prefix) cout << i << ":";
        cout << A[i] << " ";
    }
    cout << endl;
}

//--------------------------Heap sorting---------------------------------

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

void build_max_heap(int A[], int length) {
    int heap_size = length;
    for (int i = length/2-1; i >= 0; i--) {
        max_heapify(A, heap_size, i);
    }
}

void heapsort(int A[], int length) {
    build_max_heap(A, length);
    int heap_size = length;
    for (int i = length-1; i > 1; i--) {
        swap_elements(A[0], A[i]);
        heap_size = heap_size - 1;
        max_heapify(A, heap_size, 0);
    }
}

//--------------------------------------------------------------------------

int main()
{

    /*
    int start = 1'000'000;
    int step = 100'000;
    int finish = 3'000'000;

    int * ptr;

    chrono::time_point<chrono::high_resolution_clock> t1, t2;
    int x;
    cin >> x;

    for (int i = start; i <= finish; i += step) {
        ptr = new int[i];
        init_rand(ptr, i);
        //printd(ptr, i, false);
        t1 = chrono::high_resolution_clock::now();
        //bubble_sort(ptr, i);
        heapsort(ptr, i);
        t2 = chrono::high_resolution_clock::now();
        //cout << endl;
        //printd(ptr, i, false);
        auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
        cout << i << "\t" << ms_int.count() << endl;
        delete ptr;
    }

    cin >> x;




    //printd(A, 10);
    max_heapify(A, 10, 1);
    printd(A, 10);

    int B[10] = {1, 8, 2, 3, 9, 7, 14, 10, 4, 16};
    build_max_heap(B, 10);
    printd(B, 10);

    int A[10] = {16, 4, 10, 14, 7, 9, 3, 2, 8, 1};
    //build_max_heap(A, 10);
    int B[10] = {16, 1, 10, 14, 7, 9, 3, 2, 8, 1};

    heapsort(B, 10);

    printd(B, 10);
         */

    int A[] = {4, 3, 2, 1};
    double B[] = {12.01, 12.0001, 14.0, 15};
    printd<int>(A, 4);
    bubble_sort<int>(A, 4);
    printd<int>(A, 4);

    printd<double>(B, 4);
    bubble_sort<double>(B, 4);
    printd<double>(B, 4);

    return 0;
}
