cmake_minimum_required(VERSION 3.19)

project(calc-cpp)

add_compile_options(
    -std=c++17
    -Wall
    --pedantic-errors
)

add_library(parser STATIC src/parser.cpp src/lexer.cpp)
add_executable(calc-cpp src/main.cpp)
target_link_libraries(calc-cpp parser)
