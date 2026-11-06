cmake_minimum_required(VERSION 3.16)

project(Matrix_CPP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Matrix_CPP
        main.cpp
        Matrix.cpp
        Matrix.h
        ../lecture13.cpp
        ../practicetest1213.cpp
)

target_compile_options(Matrix_CPP PRIVATE
        -Wall
        -Wextra
)