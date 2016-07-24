
include_directories(.)
include_directories(..)

set(src                     graph.c graph.h)

add_library(graph           SHARED ${src})
target_link_libraries(graph avl)
