cmake_minimum_required(VERSION 2.6, FATAL_ERROR)
project(ttree C)

include_directories(${ttree_source_dir})

ADD_LIBRARY(ttree STATIC ttree.c)
#add_subdirectory(examples)
