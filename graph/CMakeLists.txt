
include_directories(.)
include_directories(..)

set(src                     graph.c graph.h)

add_definitions(${misc_CFLAGS})

add_library(graph           SHARED ${src})
target_link_libraries(graph ${misc_LDFLAGS})
