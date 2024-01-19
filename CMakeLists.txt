# Require ninja-1.11+
#
# CXX=clang++-17 CC=clang-17 /full/path/to/cmake-3.28 -GNinja -DCMAKE_MAKE_PROGRAM=/full/path/to/ninja -S . -B build
# /full/path/to/cmake-3.28 --build build

cmake_minimum_required(VERSION 3.28)
project(sat_solver CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -fconstexpr-depth=100 -fconstexpr-steps=5000000")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

include_directories(${CMAKE_BINARY_DIR})

if (CMAKE_BUILD_TYPE EQUAL "Debug")
  set(CXX_SANITIZERS
      -fsanitize=address
      -fsanitize=alignment
      -fsanitize=bool
      -fsanitize=bounds
      -fsanitize=enum
      -fsanitize=float-cast-overflow
      -fsanitize=float-divide-by-zero
      -fsanitize=integer-divide-by-zero
      -fsanitize=leak
      -fsanitize=nonnull-attribute
      -fsanitize=null
      -fsanitize=return
      -fsanitize=returns-nonnull-attribute
      -fsanitize=shift
      -fsanitize=signed-integer-overflow
      -fsanitize=undefined
      -fsanitize=unreachable
      -fsanitize=vla-bound
      -fsanitize=vptr
      -fsized-deallocation)

  add_compile_options(${CXX_SANITIZERS})
  add_link_options(${CXX_SANITIZERS})
endif()

add_subdirectory(common)
add_subdirectory(sat_runtime)
add_subdirectory(sat_comptime)
add_subdirectory(sat_hybrid)