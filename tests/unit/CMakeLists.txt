include_directories(${ttree_SOURCE_DIR})
link_directories(${ttree_SOURCE_DIR})

ADD_LIBRARY(utest SHARED utest.c)
set(UTLIB utest)

add_executable(t_init t_init.c)
target_link_libraries(t_init ttree ${UTLIB})
add_custom_target(tests DEPENDS ${UTLIB} t_init)
