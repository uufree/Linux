cmake_minimum_required(VERSION 2.8)
project(a)

set(CMAKE_C_COMPILER g++)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

ADD_DEFINITIONS("-g -Wall -o -lpthread -std=c++11")

add_executable(a main.cc Clock.cc)
target_link_libraries(a ${OpenCV_LIBS})
