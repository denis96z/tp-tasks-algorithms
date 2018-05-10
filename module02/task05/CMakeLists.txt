cmake_minimum_required(VERSION 3.10)
project(task05)

set(CMAKE_CXX_STANDARD 14)

add_executable(task05 main.cpp huffman.h huffman.cpp types.h bitcode.h huffman_tree.h)