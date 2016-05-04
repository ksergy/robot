
include_directories(.)

set(containers_src containers.c containers.h)

add_library(containers SHARED ${containers_src})
