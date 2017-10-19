CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(server)


SET(CMAKE_C_COMPILER g++)
INCLUDE_DIRECTORIES(/usr/include/pthread)

SET(SERVER Server.cc HandleEvent.cc File.cc InetAddress.cc Mutex.cc Socket.cc Thread.cc )


FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLDUE_DIRS})
ADD_DEFINITIONS("-g -Wall -o -lpthread -std=c++11")

ADD_EXECUTABLE(server ${SERVER})

TARGET_LINK_LIBRARIES(server pthread ${OpenCV_LIBS})
