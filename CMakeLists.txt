cmake_minimum_required(VERSION 3.0.0)
project(GameOfLife VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})

add_library(Grid STATIC grid.cc)
target_link_libraries(Grid PRIVATE ${PNG_LIBRARIES})

add_library(Cell STATIC cell.cc)

add_executable(GameOfLife main.cc)
target_link_libraries(GameOfLife PRIVATE Grid Cell ${PNG_LIBRARIES})
target_compile_options(GameOfLife PRIVATE -pedantic -Wall -Wextra -Werror -pedantic-errors)
target_compile_definitions(GameOfLife PRIVATE IMAGE_OUTPUT_PATH="${CMAKE_CURRENT_BINARY_DIR}/output.png")

find_package(GTest REQUIRED)

add_executable(TestRunner testRunner.cc)
target_link_libraries(TestRunner PUBLIC Grid Cell ${GTEST_LIBRARIES} pthread)

configure_file(inputLife.txt inputLife.txt COPYONLY)
configure_file(outputLife.txt outputLife.txt COPYONLY)
