# cmake_minimum_required(VERSION <specify CMake version here>)
project(C C)

set(CMAKE_C_STANDARD 99)

add_executable(C BruteForce_Linux.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")