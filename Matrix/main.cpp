#include <iostream>
#include "Matrix.h"



int main()
{
    Matrix<int> A = {{1, 1}, {1, 0}};


    for (int i = 1; i < 20; ++i) {
        A.pow(i).output(std::cout);
        std::cout << std::endl;
    }

    return 0;
}
