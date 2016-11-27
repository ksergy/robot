
include_directories(.)
include_directories(..)

set(src                     multigrid-converter.c multigrid-converter.h)

add_definitions(${misc_CFLAGS})

add_library(multigrid-converter SHARED ${src})
target_link_libraries(multigrid-converter multigrid graph)
