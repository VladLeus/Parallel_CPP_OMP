cmake_minimum_required(VERSION 3.27)
project(untitled)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(untitled main.cpp)

