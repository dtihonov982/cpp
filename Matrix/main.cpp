#include <iostream>
#include "Matrix.h"
#include <vector>
#include <utility>
#include <fstream>


int main()
{
    std::ifstream isA{"3x3.m"};
    Matrix<float> A{isA};
    std::cout << A.det() << std::endl;
#if 0
    A.output(std::cout);
    Matrix<float> B{A.minorMatrix(0, 0)};
    B.output(std::cout);
    Matrix<double> A = {{1.0, 1.0}, {1.0, 0.0}};
    A.output(std::cout);
    Matrix<double> E = {{1.0, 0.0}, {0.0, 1.0}};
    E.output(std::cout);

    Matrix<double> C = A.pow(5);
    C.output(std::cout);
    Matrix<double> B = std::move(A);
    B.output(std::cout);
    std::cout << A.pow(10) << std::endl;
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
