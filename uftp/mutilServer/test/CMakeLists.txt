CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(server)

SET(CMAKE_C_COMPILER g++)
INCLUDE_DIRECTORIES(/usr/include/pthread)

SET(SERVER EventLoopThreadTest.cc ../../Buffer.cc ../../Channel.cc ../../Epoller.cc  ../../EventLoop.cc ../../File.cc ../../Socket.cc ../../TcpConnection.cc ../../Thread.cc ../../ThreadPool.cc ../Current.cc ../EventLoopThread.cc ../EventLoopThreadPool.cc ../MutilAcceptor.cc ../MutilTcpServer.cc)

ADD_DEFINITIONS("-g -Wall -o -lpthread -std=c++11")

ADD_EXECUTABLE(server ${SERVER})

TARGET_LINK_LIBRARIES(server pthread)
