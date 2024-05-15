cmake_minimum_required(VERSION 3.10)
project(calc)
include_directories(include)

set(CMAKE_CXX_STANDARD 23)

add_executable(calc main.cpp
    ./src/scanner.cc
    ./src/parser.cc
    ./src/state.cc
    ./src/calc.cc
    ./src/functions.cc)