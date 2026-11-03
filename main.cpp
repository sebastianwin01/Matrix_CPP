//
// Created by sebastian on 11/2/26.
//
#include "Matrix.h"
#include <iostream>
int main() {
    Matrix A(2, 2);

    A.at(0, 0) = 10;
    A.at(0, 1) = 20;
    A.at(1, 0) = 30;
    A.at(1, 1) = 40;

    const Matrix& B = A;

    std::cout << A.at(0, 1) << "\n"; // 20
    std::cout << B.at(1, 0) << "\n"; // 30
    Matrix C =B;

    std::cout<< C.at(0,1) <<"\n";

    // B.at(0, 0) = 100; // Compilation error
}