
include_directories(.)
include_directories(..)

set(src                     path-finder.c path-finder.h)

add_library(path-finder     SHARED ${src})
target_link_libraries(path-finder graph)
