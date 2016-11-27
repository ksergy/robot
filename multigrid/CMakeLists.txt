
include_directories(.)
include_directories(..)

set(src                     multigrid.c multigrid.h)

add_definitions(${misc_CFLAGS})
add_library(multigrid       SHARED ${src})
target_link_libraries(multigrid ${misc_LDFLAGS})
