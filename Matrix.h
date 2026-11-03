//
// Created by sebastian on 10/31/26.
//

#ifndef C_PROJECTS_MATRIX_H
#define C_PROJECTS_MATRIX_H

#include <cstddef>
#include <stdexcept>


class Matrix {
    std::size_t rows_;
    std::size_t cols_;
    double* data_;
public:
    //Main constructor, regular constructor!
    Matrix(std::size_t rows, std::size_t cols);

    //Rule of five
    //Destructor
    ~Matrix();
    //copy constructor
    Matrix(const Matrix& other);
    //assigment constructor
    Matrix& operator=(const Matrix& other);
    //move constructor
    Matrix(Matrix&& other)noexcept;
    //move asignment
    Matrix& operator=(Matrix&& other)noexcept;

    //element access
    double& at(std::size_t row, std::size_t col);
    const double& at (std::size_t row, std::size_t col)const;

    //operator overloding for matrix operations
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator/(const Matrix& other) const;

    //boolean operators
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;

    //Display
    void print() const;

    //transpose matrix
    // Matrix_CPP transpose
    Matrix transpose() const;

};
std::ostream& operator<<(std::ostream& os, const Matrix& matrix);


#endif //C_PROJECTS_MATRIX_H