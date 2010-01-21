include_directories(${ttree_SOURCE_DIR})
link_directories(${ttree_SOURCE_DIR})

add_executable(t_init t_init.c)
target_link_libraries(t_init ttree)
add_custom_target(tests DEPENDS t_init)
