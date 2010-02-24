include_directories(${ttree_SOURCE_DIR})
link_directories(${ttree_SOURCE_DIR})

ADD_LIBRARY(utest SHARED utest.c)
set(UTLIB utest)
set(OBJS utils.c)
set(TESTS t_init t_insert_balance)

add_executable(t_init t_init.c ${OBJS})
add_executable(t_insert_balance t_insert_balance.c ${OBJS})
target_link_libraries(t_init ttree ${UTLIB})
target_link_libraries(t_insert_balance ttree ${UTLIB})
add_custom_target(tests DEPENDS ${UTLIB} ${TESTS})
