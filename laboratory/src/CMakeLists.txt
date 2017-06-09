CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(server)
PROJECT(client)

SET(CMAKE_C_COMPILER g++)
INCLUDE_DIRECTORIES(/usr/include/pthread)

SET(SERVER Server.cc ../Buffer.cc ../Channel.cc ../Epoller.cc ../EventLoop.cc ../File.cc ../Socket.cc ../TcpConnection.cc ../Thread.cc ../ThreadPool.cc ../singleServer/Acceptor.cc ../singleServer/TcpServer.cc)

SET(CLIENT Client.cc ../Buffer.cc  ../File.cc ../Socket.cc ../singleClient/SingleTcpClient.cc)


ADD_DEFINITIONS("-g -Wall -o -lpthread -std=c++11")

ADD_EXECUTABLE(server ${SERVER})
ADD_EXECUTABLE(client ${CLIENT})

TARGET_LINK_LIBRARIES(server pthread)
TARGET_LINK_LIBRARIES(client pthread)
