CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(server)
PROJECT(client)


SET(CMAKE_C_COMPILER g++)
INCLUDE_DIRECTORIES(/usr/include/pthread)

SET(SERVER Server.cc ../AsyncTcpServer.cc ../AsyncAcceptor.cc ../../Buffer.cc ../../Channel.cc ../../ChannelMap.cc ../../Condition.cc ../../Directory.cc ../../Epoller.cc ../../EventMap.cc ../../EventLoop.cc ../../File.cc ../../InetAddress.cc ../../Mutex.cc ../../Socket.cc ../../TaskPool.cc ../../TcpConnection.cc ../../TcpConnectionMap.cc ../../Thread.cc ../../ThreadPool.cc ../../Timestamp.cc ../../Timer.cc ../../TimerQueue.cc)

SET(CLIENT Client.cc  ../../Buffer.cc  ../../Condition.cc ../../Directory.cc ../../File.cc ../../InetAddress.cc ../../Mutex.cc ../../Socket.cc)

FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLDUE_DIRS})
ADD_DEFINITIONS("-g -Wall -o -lpthread -std=c++11")

ADD_EXECUTABLE(server ${SERVER})
ADD_EXECUTABLE(client ${CLIENT})

TARGET_LINK_LIBRARIES(server pthread ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(client pthread ${OpenCV_LIBS})
