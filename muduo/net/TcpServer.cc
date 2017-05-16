// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include <muduo/net/TcpServer.h>

#include <muduo/base/Logging.h>
#include <muduo/net/Acceptor.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/SocketsOps.h>

#include <boost/bind.hpp>

#include <stdio.h>  // snprintf

using namespace muduo;
using namespace muduo::net;

TcpServer::TcpServer(EventLoop* loop,
                     const InetAddress& listenAddr,
                     const string& nameArg,
                     Option option)
  : loop_(CHECK_NOTNULL(loop)),
    ipPort_(listenAddr.toIpPort()),
    name_(nameArg),
    acceptor_(new Acceptor(loop, listenAddr, option == kReusePort)),
    threadPool_(new EventLoopThreadPool(loop, name_)),
    connectionCallback_(defaultConnectionCallback),
    messageCallback_(defaultMessageCallback),
    nextConnId_(1)
{//注册默认的信息处理以及连接callback
  acceptor_->setNewConnectionCallback(
      boost::bind(&TcpServer::newConnection, this, _1, _2));//注册给acceptor的回调
}

TcpServer::~TcpServer()
{
  loop_->assertInLoopThread();//断言是否在IO线程
  LOG_TRACE << "TcpServer::~TcpServer [" << name_ << "] destructing";

  for (ConnectionMap::iterator it(connections_.begin());
      it != connections_.end(); ++it)
  {//遍历Map
    TcpConnectionPtr conn = it->second;
    it->second.reset();//注销map中的con，然后再用conn注销和conn自身有关的信息
    conn->getLoop()->runInLoop(
      boost::bind(&TcpConnection::connectDestroyed, conn));//自身注销掉channel？？为什么要强行加入conn
    conn.reset();//再次注销
  }
}

void TcpServer::setThreadNum(int numThreads)
{//设置线程池的开始数目
  assert(0 <= numThreads);
  threadPool_->setThreadNum(numThreads);
}

void TcpServer::start()
{//TcpServer开始工作
  if (started_.getAndSet(1) == 0)//获得原子计数
  {
    threadPool_->start(threadInitCallback_);//线程池开始工作

    assert(!acceptor_->listenning());//打开accepor的监听状态
    loop_->runInLoop(
        boost::bind(&Acceptor::listen, get_pointer(acceptor_)));//打开acceptor的listening
  }
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{//将本函数注册个acceptor
  loop_->assertInLoopThread();//断言是否在IO线程
  EventLoop* ioLoop = threadPool_->getNextLoop();//获得线程池中的一个loop
  char buf[64];//获得线程池map中的string索引
  snprintf(buf, sizeof buf, "-%s#%d", ipPort_.c_str(), nextConnId_);
  ++nextConnId_;
  string connName = name_ + buf;

  LOG_INFO << "TcpServer::newConnection [" << name_
           << "] - new connection [" << connName
           << "] from " << peerAddr.toIpPort();
  InetAddress localAddr(sockets::getLocalAddr(sockfd));//获得本地的地址，用于构建Connection
  // FIXME poll with zero timeout to double confirm the new connection
  // FIXME use make_shared if necessary
  TcpConnectionPtr conn(new TcpConnection(ioLoop,
                                          connName,
                                          sockfd,
                                          localAddr,
                                          peerAddr));//构建了一个connection
  connections_[connName] = conn;//将新构建的con加入server的map中
  conn->setConnectionCallback(connectionCallback_);//muduo默认的
  conn->setMessageCallback(messageCallback_);//moduo默认的
  conn->setWriteCompleteCallback(writeCompleteCallback_);//？？
  conn->setCloseCallback(
      boost::bind(&TcpServer::removeConnection, this, _1)); // FIXME: unsafe
  ioLoop->runInLoop(boost::bind(&TcpConnection::connectEstablished, conn));//在某个线程池的loop中加入这个con
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
  // FIXME: unsafe
  loop_->runInLoop(boost::bind(&TcpServer::removeConnectionInLoop, this, conn));//注册到loop线程中移除这个con
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
  loop_->assertInLoopThread();//断言是否在IO线程
  LOG_INFO << "TcpServer::removeConnectionInLoop [" << name_
           << "] - connection " << conn->name();
  size_t n = connections_.erase(conn->name());//删除该con
  (void)n;
  assert(n == 1);
  EventLoop* ioLoop = conn->getLoop();//获得线程Loop
  ioLoop->queueInLoop(
      boost::bind(&TcpConnection::connectDestroyed, conn));//将线程销毁动作添加到loop中去
}

