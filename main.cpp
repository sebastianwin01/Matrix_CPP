//
// Created by sebastian on 11/2/26.
//
#include "Matrix.h"
#include <iostream>

#include <iostream>
#include "Matrix.h"

// Helper function to display a matrix
void printMatrix(const Matrix& matrix) {
    for (std::size_t i = 0; i < matrix.rows(); ++i) {
        for (std::size_t j = 0; j < matrix.cols(); ++j) {
            std::cout << matrix.at(i, j) << "\t";
        }
        std::cout << '\n';
    }
}

int main() {
    Matrix A(2, 2);
    Matrix B(2, 2);

    // Initialize Matrix A
    A.at(0, 0) = 1;
    A.at(0, 1) = 2;
    A.at(1, 0) = 3;
    A.at(1, 1) = 4;

    // Initialize Matrix B
    B.at(0, 0) = 5;
    B.at(0, 1) = 6;
    B.at(1, 0) = 7;
    B.at(1, 1) = 8;

    // Display original matrices
    std::cout << "Matrix A:\n";
    printMatrix(A);

    std::cout << "\nMatrix B:\n";
    printMatrix(B);

    // Addition
    Matrix C = A;
    C += B;

    std::cout << "\nC = A + B:\n";
    printMatrix(C);

    // Subtraction
    Matrix D = A;
    D -= B;

    std::cout << "\nD = A - B:\n";
    printMatrix(D);

    // Multiplication
    Matrix E = A;
    E *= B;

    std::cout << "\nE = A * B:\n";
    printMatrix(E);

    return 0;
}
