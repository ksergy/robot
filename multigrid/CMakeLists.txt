
include_directories(.)
include_directories(..)

set(src                     multigrid.c multigrid.h)

add_library(multigrid       SHARED ${src})
target_link_libraries(multigrid containers)
