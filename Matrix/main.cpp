#include <iostream>
#include "Matrix.h"
#include <vector>
#include <utility>



int main()
{
    Matrix<float> A = {{1.0, 1.0}, {1.0, 0.0}};
    std::cout << A.pow(10) << std::endl;


#if 0
    std::vector<Matrix<float>> vM;
    vM.push_back(A);
    std::cout << "---" << std::endl;
    vM.push_back(A);
    std::cout << "---" << std::endl;
    vM.push_back(A);
    std::cout << "---" << std::endl;

    std::cout << A.pow(2) << std::endl;
    Matrix<int> B = {{5, 6}, {7, 8}};
    std::cout << B << std::endl;
    Matrix<int> C = A.product(B);
    std::cout << A.product(B) << std::endl;


    for (int i = 1; i < 20; ++i) {
        A.pow(i).output(std::cout);
        std::cout << std::endl;
    }
#endif // 0

    return 0;
}
