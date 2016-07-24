project(robot)
cmake_minimum_required(VERSION 2.8)
add_definitions(-g -Wall -Werror)
include_directories(.)

add_subdirectory(lib)
add_subdirectory(tests)
add_subdirectory(multigrid)
add_subdirectory(graph)

add_executable(main main.c)
target_link_libraries(main multigrid)
