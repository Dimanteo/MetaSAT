# Require ninja-1.11+
#
# CXX=clang++-17 CC=clang-17 /full/path/to/cmake-3.28 -GNinja -DCMAKE_MAKE_PROGRAM=/full/path/to/ninja -S . -B build
# /full/path/to/cmake-3.28 --build build

cmake_minimum_required(VERSION 3.28)
project(sat_solver CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(common)
add_subdirectory(sat_runtime)