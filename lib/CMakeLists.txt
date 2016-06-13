
include_directories(.)

set(containers_src          containers.c containers.h)
set(avl_src                 avl-tree.c avl-tree.h)
set(hash_src                hash-functions.c hash-functions.h
                            hash-map.c hash-map.h)
set(set_src                 set.c set.h)

add_library(containers      SHARED ${containers_src})
add_library(avl             SHARED ${avl_src})
add_library(hash-map        SHARED ${hash_src})
add_library(set             SHARED ${set_src})

target_link_libraries(hash-map avl containers)
