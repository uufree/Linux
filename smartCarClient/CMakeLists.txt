CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(client)


SET(CMAKE_C_COMPILER g++)
INCLUDE_DIRECTORIES(/usr/include/pthread)


SET(CLIENT Client.cc File.cc InetAddress.cc Mutex.cc Socket.cc i18nText.cc)

FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLDUE_DIRS})
ADD_DEFINITIONS("-g -Wall -o -lpthread -std=c++11")

ADD_EXECUTABLE(client ${CLIENT})

TARGET_LINK_LIBRARIES(client pthread ${OpenCV_LIBS} freetype)
