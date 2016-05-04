project(robot)
cmake_minimum_required(VERSION 2.8)
add_definitions(-g -O0)
include_directories(.)

add_subdirectory(lib)
add_subdirectory(tests)

