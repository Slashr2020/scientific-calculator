cmake_minimum_required(VERSION 3.11)
project(RPNCalculator C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCES src/main.c src/rpn.c src/tokens.c src/stack.c )

add_executable(calculator_app ${SOURCES})
target_include_directories(calculator_app PRIVATE src)

find_library(MATH_LIBRARY m)
target_link_libraries(calculator_app PRIVATE ${MATH_LIBRARY})

