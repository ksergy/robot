
include_directories(.)
include_directories(..)

set(src                     multigrid-converter.c multigrid-converter.h)

add_library(multigrid-converter SHARED ${src})
target_link_libraries(multigrid-converter multigrid graph)
