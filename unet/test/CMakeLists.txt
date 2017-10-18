CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(a)

SET(CMAKE_C_COMPILER g++)
INCLUDE_DIRECTORIES(/usr/include/pthread)

SET(SERVER DirectoryTest.cc ../File.cc ../Directory.cc)

ADD_DEFINITIONS("-g -Wall -o -lpthread -std=c++11")

ADD_EXECUTABLE(a ${SERVER})

TARGET_LINK_LIBRARIES(a pthread)
