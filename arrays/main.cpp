#include "stdio.h"

void bubble_sort(int A[], int length) {
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

int sum(int A[], int n) {
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += A[i];
    }
    return sum;
}

int main() {
    int A[] = {16,4,10,14,7,9,3,2,8,1};
    printf("%d", sum(A, 10));
    return 0;
}

