cmake_minimum_required(VERSION 2.6, FATAL_ERROR)
project(ttree C)

set(DEFAULT_CFLAGS "-O3")
set(GCOV_CFLAGS "-g -fprofile-arcs -ftest-coverage")
option(WITH_GCOV "Use GCOV" OFF)

if(WITH_GCOV)
  set(CMAKE_C_FLAGS "${GCOV_CFLAGS}")
else()
  set(CMAKE_C_FLAGS "${DEFAULT_CFLAGS}")
endif()

include_directories(${ttree_source_dir})
ADD_LIBRARY(ttree STATIC ttree.c)
add_subdirectory(tests/unit EXCLUDE_FROM_ALL)

