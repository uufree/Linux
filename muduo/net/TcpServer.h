// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_TCPSERVER_H
#define MUDUO_NET_TCPSERVER_H

#include <muduo/base/Atomic.h>
#include <muduo/base/Types.h>
#include <muduo/net/TcpConnection.h>

#include <map>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace muduo
{
namespace net
{

class Acceptor;
class EventLoop;
class EventLoopThreadPool;

///
/// TCP server, supports single-threaded and thread-pool models.
///
/// This is an interface class, so don't expose too much details.
class TcpServer : boost::noncopyable
{
 public:
  typedef boost::function<void(EventLoop*)> ThreadInitCallback;
  enum Option//server的两种状态
  {
    kNoReusePort,
    kReusePort,
  };

  //TcpServer(EventLoop* loop, const InetAddress& listenAddr);
  TcpServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg,
            Option option = kNoReusePort);
  ~TcpServer();  // force out-line dtor, for scoped_ptr members.

  const string& ipPort() const { return ipPort_; }//获得一些记录功能
  const string& name() const { return name_; }
  EventLoop* getLoop() const { return loop_; }//获得server的loop

  /// Set the number of threads for handling input.
  ///
  /// Always accepts new connection in loop's thread.
  /// Must be called before @c start
  /// @param numThreads
  /// - 0 means all I/O in loop's thread, no thread will created.
  ///   this is the default value.
  /// - 1 means all I/O in another thread.
  /// - N means a thread pool with N threads, new connections
  ///   are assigned on a round-robin basis.
  void setThreadNum(int numThreads);//设置线程池开启的线程数目
  void setThreadInitCallback(const ThreadInitCallback& cb)
  { threadInitCallback_ = cb; }//设置线程池的Init线程，用户设置
  /// valid after calling start()
  boost::shared_ptr<EventLoopThreadPool> threadPool()
  { return threadPool_; }//获得指向线程池的shared_ptr

  /// Starts the server if it's not listenning.
  ///
  /// It's harmless to call it multiple times.
  /// Thread safe.
  void start();//server开始运行

  /// Set connection callback.
  /// Not thread safe.
  void setConnectionCallback(const ConnectionCallback& cb)
  { connectionCallback_ = cb; }//设置连接回调

  /// Set message callback.
  /// Not thread safe.
  void setMessageCallback(const MessageCallback& cb)
  { messageCallback_ = cb; }//设置处理信息的回调

  /// Set write complete callback.
  /// Not thread safe.
  void setWriteCompleteCallback(const WriteCompleteCallback& cb)
  { writeCompleteCallback_ = cb; }//设置写事件完成的回调

 private:
  /// Not thread safe, but in loop
  void newConnection(int sockfd, const InetAddress& peerAddr);//注册给Acceptor的回调函数
  /// Thread safe.
  void removeConnection(const TcpConnectionPtr& conn);//移除connettion
  /// Not thread safe, but in loop
  void removeConnectionInLoop(const TcpConnectionPtr& conn);//将移除动作添加到IO线程中

  typedef std::map<string, TcpConnectionPtr> ConnectionMap;//核心存储的物品

  EventLoop* loop_;  // the acceptor loop
  const string ipPort_;
  const string name_;
  boost::scoped_ptr<Acceptor> acceptor_; //唯一持有的Acceptor
  boost::shared_ptr<EventLoopThreadPool> threadPool_;//持有的线程池
  ConnectionCallback connectionCallback_;//各种回调
  MessageCallback messageCallback_;
  WriteCompleteCallback writeCompleteCallback_;
  ThreadInitCallback threadInitCallback_;
  AtomicInt32 started_;//原子计数，一般用于输出有顺序的STRING
  // always in loop thread
  int nextConnId_;//用于获得线程池中的loop
  ConnectionMap connections_;//核心存储部分
};

}
}

#endif  // MUDUO_NET_TCPSERVER_H
