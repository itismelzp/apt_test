cmake_minimum_required(VERSION 3.19)
project(leetcode)

set(CMAKE_CXX_STANDARD 11)

add_executable(leetcode
        leetcode-utils.hpp
        binaryTree.h
        validateStackSequences946.cpp
        )