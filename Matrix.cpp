//
// Created by sebastian on 10/31/26.
//

#include "Matrix.h"
//main constructor define the space of the matrix in the heap memery
Matrix::Matrix(std::size_t rows, std::size_t cols)
    :rows_(rows), cols_(cols)
{
    data_ = new double [rows * cols]{};
}

//1. Destructor first of the rule of five, explicitly delete information from heap memory
Matrix::~Matrix() {
    delete[] data_;
}
//2. Copy constructor
Matrix::Matrix(const Matrix& other)
    :rows_(other.rows_), cols_(other.cols_),data_(new double[rows_ *cols_])
{
    for (size_t i=0; i < rows_ ; i++) {
        for (size_t j=0; j<cols_; j++) {
            data_[i * cols_+j]= other.data_[i * cols_ + j];
        }
    }
}
//3. Assignment operator = overload
Matrix& Matrix::operator=(const Matrix& other) {
    if (this != & other) {
        double* newData = new double [other.rows_* other.cols_];
        for (size_t i = 0;i< other.rows_; i++ ) {
            for (size_t j= 0; j < other.cols_; j++) {
                newData[i * other.cols_ + j] = other.data_[i * other.cols_ +j];
            }
        }
        delete[] data_;
        rows_= other.rows_;
        cols_= other.cols_;
        data_= newData;
    }
    return *this;
}
//4. Move constructor
Matrix::Matrix (Matrix && other) noexcept
    :rows_(other.rows_), cols_(other.cols_), data_(other.data_)
{
    other.rows_ = 0;
    other.cols_ = 0;
    other.data_ = nullptr;
}
//5. Move assigment constructor
Matrix&Matrix::operator=(Matrix&& other) noexcept
{
    if (this != &other)
    {
        delete[] data_;
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = other.data_;
        other.rows_ = 0;
        other.cols_ = 0;
        other.data_ = nullptr;
    }
    return *this;
}

double& Matrix::at(std::size_t row, std::size_t col) {
    if (row>= rows_ || col >= cols_) {
        throw std::out_of_range("Matrix_CPP index out of bounds");
    }
    return data_[row * cols_ + col];

}

const double& Matrix::at(std::size_t row, std::size_t col) const {
    if (row >= rows_ || col >= cols_) {
        throw std::out_of_range("Matrix_CPP index out of bounds:");
    }
    return data_[row * cols_ + col];
}
Matrix Matrix::operator+(const Matrix& other) const{
    // check matrix dimentions
    if (other.rows_ != rows_ || other.cols_ != cols_) {
        throw std::out_of_range("Matrices have to be the same size");
    }
    Matrix result(rows_,cols_);
    for (size_t i =0; i<rows_;i++) {
        for (size_t j = 0; j < cols_; j++) {
            result.at(i, j) = at(i, j) + other.at(i, j);
        }
    }
    return result;
}























