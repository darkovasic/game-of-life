cmake_minimum_required(VERSION 3.0.0)
project(GameOfLife VERSION 0.1.0)

add_compile_options(-std=c++17 -pedantic -Wall -Wextra -Werror -pedantic-errors)

find_package(GTest REQUIRED)
message("GTEST_INCLUDE_DIRS = ${GTEST_INCLUDE_DIRS}")

add_library(Grid STATIC grid.cc)
add_library(Cell STATIC cell.cc)

add_executable(GameOfLife main.cc)
target_link_libraries(GameOfLife PUBLIC Grid Cell)

add_executable(TestRunner testRunner.cc)
target_link_libraries(TestRunner PUBLIC Grid Cell ${GTEST_LIBRARIES} pthread)

configure_file(inputLife.txt inputLife.txt COPYONLY)