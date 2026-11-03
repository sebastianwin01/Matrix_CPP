# Matrix Class in C++

A C++17 implementation of a dynamically allocated, two-dimensional matrix using **contiguous heap storage**, **row-major indexing**, and the **Rule of Five**. The project focuses on manual resource management, const-correct element access, and in-place matrix arithmetic.

## Features

- Constructs a matrix with a specified number of rows and columns, initially filled with zeros.
- Stores elements in a contiguous, dynamically allocated `double` array.
- Implements all five special member functions for ownership and copy/move semantics.
- Provides bounds-checked element access with both mutable and const overloads.
- Exposes read-only dimension accessors: `rows()` and `cols()`.
- Supports compound addition (`+=`), subtraction (`-=`), and mathematical matrix multiplication (`*=`).
- Throws standard exceptions for invalid indices and incompatible matrix dimensions.

## Internal representation

The class maintains three private data members:

```cpp
std::size_t rows_; // Number of rows
std::size_t cols_; // Number of columns
double* data_;     // Contiguous array of rows_ * cols_ elements
```

A matrix with `m` rows and `n` columns is stored as a one-dimensional array of `m * n` doubles, in **row-major order**. The element at row `i`, column `j` is located at:

```cpp
data_[i * cols_ + j]
```

For example, the matrix

```text
[ 1  2  3 ]
[ 4  5  6 ]
```

is stored in memory as:

```text
[ 1, 2, 3, 4, 5, 6 ]
```

The constructor allocates and value-initializes the array:

```cpp
Matrix::Matrix(std::size_t rows, std::size_t cols)
    : rows_(rows), cols_(cols) {
    data_ = new double[rows * cols]{};
}
```

## Rule of Five

Because `Matrix` owns a raw heap allocation, it implements all five resource-management operations:

| Operation | Signature | Purpose |
| --- | --- | --- |
| Destructor | `~Matrix()` | Releases the owned array with `delete[]`. |
| Copy constructor | `Matrix(const Matrix& other)` | Allocates independent storage and copies every element. |
| Copy assignment | `Matrix& operator=(const Matrix& other)` | Copies another matrix into an existing one, accounting for self-assignment. |
| Move constructor | `Matrix(Matrix&& other) noexcept` | Transfers ownership of the allocation without copying the elements. |
| Move assignment | `Matrix& operator=(Matrix&& other) noexcept` | Frees existing storage, takes ownership of another matrix's array, and clears the source. |

Following a move, the source matrix has `rows_ == 0`, `cols_ == 0`, and `data_ == nullptr`. It can be destroyed or assigned another value safely.

The copy assignment implementation allocates and fills a replacement array **before** deleting the old one. This preserves the current object if allocation fails, and also handles `A = A` correctly.

## Public interface shown in this implementation

```cpp
// Construction and ownership
Matrix(std::size_t rows, std::size_t cols);
~Matrix();
Matrix(const Matrix& other);
Matrix& operator=(const Matrix& other);
Matrix(Matrix&& other) noexcept;
Matrix& operator=(Matrix&& other) noexcept;

// Dimensions
std::size_t rows() const;
std::size_t cols() const;

// Bounds-checked access
double& at(std::size_t row, std::size_t col);
const double& at(std::size_t row, std::size_t col) const;

// In-place arithmetic
Matrix& operator+=(const Matrix& other);
Matrix& operator-=(const Matrix& other);
Matrix& operator*=(const Matrix& other);
```

These are the functions present in the implementation supplied for this README. Ordinary non-member `+`, `-`, and `*` operators, a stream insertion `operator<<`, and matrix division are **not included** in that code.

## Element access and const correctness

`at(row, col)` checks the index before accessing storage. It throws `std::out_of_range` when `row >= rows_` or `col >= cols_`.

```cpp
Matrix A(2, 2);
A.at(0, 1) = 5.0;             // Mutable access

const Matrix& B = A;
double value = B.at(0, 1);  // Const access: reading is allowed
// B.at(0, 1) = 9.0;         // Compilation error
```

The `rows()` and `cols()` getters are also `const`, so callers can examine dimensions without changing the matrix.

## Arithmetic operations

### Addition: `operator+=`

Adds corresponding elements to the left-hand matrix:

```cpp
A += B;
```

Both matrices must have the same number of rows **and** columns. Otherwise, the method throws `std::invalid_argument`. The operation modifies `A` and returns `*this` by reference.

### Subtraction: `operator-=`

Subtracts corresponding elements from the left-hand matrix:

```cpp
A -= B;
```

Both matrices must have the same dimensions. **Correction required in the supplied implementation:** replace the incorrect comparison `cols_ != other.rows_` with `cols_ != other.cols_` in the dimension check.

```cpp
if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument("Matrix dimensions must match");
}
```

Without this correction, some incompatible matrices may pass the check, and some compatible non-square matrices may be rejected.

### Matrix multiplication: `operator*=`

Performs **mathematical matrix multiplication**, not element-wise multiplication:

```cpp
A *= B;
```

For an `m × n` matrix `A` and an `n × p` matrix `B`, the result has shape `m × p`. Multiplication is allowed only when `A.cols() == B.rows()`.

The method:

1. Checks compatible dimensions.
2. Constructs a zero-initialized result matrix of shape `rows_ × other.cols_`.
3. Uses three nested loops to compute each result entry as a row-by-column dot product.
4. Moves the completed result into `*this` via `*this = std::move(result)`.
5. Returns `*this` by reference.

Because the result is calculated before replacing `*this`, even `A *= A` can work for a compatible square matrix. This operation can also change the number of columns in the left-hand matrix.

## Example usage

The following `main.cpp` works with the interface listed above and does **not** require `operator<<` or non-member arithmetic operators:

```cpp
#include <iostream>
#include "Matrix.h"

void printMatrix(const Matrix& matrix) {
    for (std::size_t i = 0; i < matrix.rows(); ++i) {
        for (std::size_t j = 0; j < matrix.cols(); ++j) {
            std::cout << matrix.at(i, j) << '\t';
        }
        std::cout << '\n';
    }
}

int main() {
    Matrix A(2, 2);
    Matrix B(2, 2);

    A.at(0, 0) = 1; A.at(0, 1) = 2;
    A.at(1, 0) = 3; A.at(1, 1) = 4;

    B.at(0, 0) = 5; B.at(0, 1) = 6;
    B.at(1, 0) = 7; B.at(1, 1) = 8;

    Matrix C = A; // Deep copy of A
    C += B;

    Matrix D = A; // Deep copy of A
    D -= B;

    Matrix E = A; // Deep copy of A
    E *= B;

    std::cout << "A:\n"; printMatrix(A);
    std::cout << "\nB:\n"; printMatrix(B);
    std::cout << "\nA += B (C):\n"; printMatrix(C);
    std::cout << "\nA -= B (D):\n"; printMatrix(D);
    std::cout << "\nA *= B (E):\n"; printMatrix(E);
}
```

Expected arithmetic results:

```text
A += B (C):
6   8
10  12

A -= B (D):
-4  -4
-4  -4

A *= B (E):
19  22
43  50
```

The original `A` and `B` remain unchanged in this example because the operations run on independent copies.

## Build

The project targets **C++17**. With `Matrix.h`, `Matrix.cpp`, and `main.cpp` in the same directory, a simple build command is:

```bash
c++ -std=c++17 -Wall -Wextra main.cpp Matrix.cpp -o matrix_demo
./matrix_demo
```

It can also be built from a properly configured CMake project in CLion. The implementation should include the required standard headers, such as `<cstddef>` for `std::size_t`, `<stdexcept>` for exceptions, and `<utility>` for `std::move`.

## Complexity overview

Let `m × n` be the size of the current matrix, and `n × p` be the compatible right-hand operand for multiplication.

| Operation | Time complexity | Additional storage |
| --- | --- | --- |
| Element access, `at()` | `O(1)` | `O(1)` |
| Dimension access, `rows()` / `cols()` | `O(1)` | `O(1)` |
| Copy construction / copy assignment | `O(mn)` | `O(mn)` |
| Move construction / move assignment | `O(1)` | `O(1)` |
| `+=`, `-=` | `O(mn)` | `O(1)` |
| `*=` | `O(mnp)` | `O(mp)` |

`*=` uses a new result matrix because matrix multiplication cannot generally update the left-hand operand element by element without overwriting values needed for later calculations.

## Implementation notes and possible improvements

- **Fix the subtraction dimension check** before testing non-square matrices (`cols_ != other.cols_`).
- Add non-member `operator+`, `operator-`, and `operator*` using their compound counterparts when you want the syntax `Matrix C = A + B`.
- Add `operator<<` to print matrices directly with `std::cout`.
- Add unit tests for rectangular matrices, incompatible dimensions, out-of-range access, self-assignment, self-multiplication, and copy/move behavior.
- Consider replacing manual `new[]`/`delete[]` ownership with `std::vector<double>` for production code. The raw pointer here is useful for practicing the Rule of Five.
- For performance-critical numerical work, compare cache-aware multiplication algorithms and established matrix libraries.

## Learning goals

This project practices dynamic memory management, deep copying, ownership transfer, exception safety, const correctness, operator overloading, two-dimensional indexing through contiguous storage, and matrix arithmetic in modern C++.
