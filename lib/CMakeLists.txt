
include_directories(.)

file(GLOB_RECURSE containers_src *.c *.h)

add_library(containers SHARED ${containers_src})
