project(robot C)
cmake_minimum_required(VERSION 2.8)
add_definitions(-g -Wall -Werror)
include_directories(.)

option(WITH_COVERAGE "Enable profiling and coverage report analysis" OFF)

if (WITH_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -ftest-coverage -fprofile-arcs")
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -lgcov -ftest-coverage -fprofile-arcs"
        CACHE INTERNAL "" FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -lgcov -ftest-coverage -fprofile-arcs"
        CACHE INTERNAL "" FORCE)
endif (WITH_COVERAGE)

enable_testing()

add_subdirectory(tests)
add_subdirectory(multigrid)
add_subdirectory(graph)
add_subdirectory(multigrid-converter)
add_subdirectory(path-finder)

add_executable(main main.c)
target_link_libraries(main multigrid multigrid-converter)
