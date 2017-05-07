cmake_minimum_required (VERSION 2.8)

project(thread)

set(CMAKE_C_COMPILER g++)
include_directories(/usr/include/pthread)

set(THREAD ./Thread.cc ./ThreadPool.cc)

add_definitions("-g -Wall -o -lpthread -std=c++11")

add_library(thread ${THREAD})

target_link_libraries(thread pthread)
