
include(FindPkgConfig)

pkg_check_modules(check REQUIRED check)

include_directories(.)
file(GLOB_RECURSE tests_src *.c *.h)
add_executable(tests ${tests_src})

set_target_properties(tests PROPERTIES COMPILE_FLAGS "${check_CFLAGS} -g -O0")
target_link_libraries(tests containers avl hash-map set ${check_LDFLAGS})
