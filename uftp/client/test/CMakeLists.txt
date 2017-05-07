CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(client)

SET(CMAKE_C_COMPILER g++)
INCLUDE_DIRECTORIES(/usr/include/pthread)

SET(CLIENT TcpClientTest.cc ../../Buffer.cc ../../Channel.cc ../../Epoller.cc ../../EventLoop.cc ../../File.cc ../../Socket.cc ../../TcpConnection.cc ../../Thread.cc ../../ThreadPool.cc ../Connector.cc ../TcpClient.cc)

ADD_DEFINITIONS("-g -Wall -o -lpthread -std=c++11")

ADD_EXECUTABLE(client ${CLIENT})

TARGET_LINK_LIBRARIES(client pthread)
