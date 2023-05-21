cmake_minimum_required(VERSION 3.0.0)
project(GameOfLife VERSION 0.1.0)

add_library(Grid STATIC grid.cc)
add_library(Cell STATIC cell.cc)

add_executable(GameOfLife main.cc)

target_link_libraries(GameOfLife PUBLIC Grid Cell)

# include(CTest)
# enable_testing()

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
