CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(server)
PROJECT(client)

SET(CMAKE_C_COMPILER g++)
INCLUDE_DIRECTORIES(/usr/include/pthread)

SET(SERVER Server.cc ../../Buffer.cc ../../Channel.cc ../../Epoller.cc ../../EventLoop.cc ../../File.cc ../../Socket.cc ../../TcpConnection.cc ../../Thread.cc ../../ThreadPool.cc ../../mutilServer/Current.cc ../../mutilServer/EventLoopThread.cc ../../mutilServer/EventLoopThreadPool.cc
 ../../mutilServer/MutilAcceptor.cc ../../mutilServer/MutilTcpServer.cc)


SET(CLIENT Client.cc ../../Buffer.cc ../../Channel.cc ../../ThreadPool.cc ../../Epoller.cc ../../EventLoop.cc ../../File.cc ../../Socket.cc ../../TcpConnection.cc ../../Thread.cc ../../ThreadPool.cc ../../client/Connector.cc  ../../client/TcpClient.cc ../../client/AsyncEventLoop.cc)


ADD_DEFINITIONS("-g -Wall -o -lpthread -std=c++11")

ADD_EXECUTABLE(server ${SERVER})
ADD_EXECUTABLE(client ${CLIENT})

TARGET_LINK_LIBRARIES(server pthread)
TARGET_LINK_LIBRARIES(client pthread)
