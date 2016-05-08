
include_directories(.)

set(containers_src containers.c containers.h)
set(avl_src avl-tree.c avl-tree.h)

add_library(containers      SHARED ${containers_src})
add_library(avl             SHARED ${avl_src})
