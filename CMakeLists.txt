cmake_minimum_required(VERSION 3.28)
project(sat_solver CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(CNF EXCLUDE_FROM_ALL)

target_sources(CNF PUBLIC FILE_SET CXX_MODULES FILES CNF.cppm)

add_executable(rt_sat main.cpp)

target_link_libraries(rt_sat CNF)

# Require ninja-1.11+
#
# CXX=clang++-17 CC=clang-17 /full/path/to/cmake-3.28 -GNinja -DCMAKE_MAKE_PROGRAM=/full/path/to/ninja -S . -B build
# /full/path/to/cmake-3.28 --build build