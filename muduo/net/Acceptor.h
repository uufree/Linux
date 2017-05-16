// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_ACCEPTOR_H
#define MUDUO_NET_ACCEPTOR_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <muduo/net/Channel.h>
#include <muduo/net/Socket.h>

namespace muduo
{
namespace net
{

class EventLoop;
class InetAddress;

///
/// Acceptor of incoming TCP connections.
///
class Acceptor : boost::noncopyable
{
 public:
  typedef boost::function<void (int sockfd,
                                const InetAddress&)> NewConnectionCallback;

  Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
  ~Acceptor();

  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { newConnectionCallback_ = cb; }//注册回调

  bool listenning() const { return listenning_; }//判断是否在监听
  void listen();//开始监听

 private:
  void handleRead();//处理监听字的可读选项

  EventLoop* loop_;//本身所在的LOOP
  Socket acceptSocket_;//监听字
  Channel acceptChannel_;//监听字的分发机制              
  NewConnectionCallback newConnectionCallback_;//处理新连接的回调
  bool listenning_;//是否在监听
  int idleFd_;//用来处理没有足够FD的情况
};
}
}

#endif  // MUDUO_NET_ACCEPTOR_H
