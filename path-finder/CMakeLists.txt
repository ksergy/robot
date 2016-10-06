
include_directories(.)
include_directories(..)

set(src                     path-finder.c path-finder.h
                            private/bfs-dfs.c
                            private/bfs.c private/dfs.c)

add_library(path-finder     SHARED ${src})
target_link_libraries(path-finder graph)
