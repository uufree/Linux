// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_TCPCONNECTION_H
#define MUDUO_NET_TCPCONNECTION_H

#include <muduo/base/StringPiece.h>
#include <muduo/base/Types.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/InetAddress.h>

#include <boost/any.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

// struct tcp_info is in <netinet/tcp.h>
struct tcp_info;

namespace muduo
{
namespace net
{

class Channel;
class EventLoop;
class Socket;

///
/// TCP connection, for both client and server usage.
///
/// This is an interface class, so don't expose too much details.
class TcpConnection : boost::noncopyable,
                      public boost::enable_shared_from_this<TcpConnection>
{
 public:
  /// Constructs a TcpConnection with a connected sockfd
  ///
  /// User should not create this object.
  TcpConnection(EventLoop* loop,
                const string& name,
                int sockfd,
                const InetAddress& localAddr,
                const InetAddress& peerAddr);//因为具有发送的功能，所以需要保存客户端与服务端的地址，会根据连接好的sockfd创建一个con
  ~TcpConnection();

  EventLoop* getLoop() const { return loop_; }//返回本con持有的loop
  const string& name() const { return name_; }//返回name
  const InetAddress& localAddress() const { return localAddr_; }
  const InetAddress& peerAddress() const { return peerAddr_; }
  bool connected() const { return state_ == kConnected; }//是否连接打开
  bool disconnected() const { return state_ == kDisconnected; }//是否连接关闭
  // return true if success.
  bool getTcpInfo(struct tcp_info*) const;//一些关于记录的操作
  string getTcpInfoString() const;

  // void send(string&& message); // C++11
  void send(const void* message, int len);//发送功能
  void send(const StringPiece& message);
  // void send(Buffer&& message); // C++11
  void send(Buffer* message);  // this one will swap data
  void shutdown(); // NOT thread safe, no simultaneous calling
  // void shutdownAndForceCloseAfter(double seconds); // NOT thread safe, no simultaneous calling
  void forceClose();//强行关闭
  void forceCloseWithDelay(double seconds);//延迟关闭
  void setTcpNoDelay(bool on);//是否开启NALAY算法
  void startRead();//开始读
  void stopRead();//停止读
  bool isReading() const { return reading_; }; // NOT thread safe, may race with start/stopReadInLoop 

  void setContext(const boost::any& context)//？？
  { context_ = context; }

  const boost::any& getContext() const//？？
  { return context_; }

  boost::any* getMutableContext()//？？
  { return &context_; }

  void setConnectionCallback(const ConnectionCallback& cb)
  { connectionCallback_ = cb; }//连接回调

  void setMessageCallback(const MessageCallback& cb)
  { messageCallback_ = cb; }//信息回调

  void setWriteCompleteCallback(const WriteCompleteCallback& cb)
  { writeCompleteCallback_ = cb; }//写完全回调

  void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t highWaterMark)
  { highWaterMarkCallback_ = cb; highWaterMark_ = highWaterMark; }//超过一定位置的回调

  /// Advanced interface
  Buffer* inputBuffer()//返回BUFFER
  { return &inputBuffer_; }

  Buffer* outputBuffer()
  { return &outputBuffer_; }

  /// Internal use only.
  void setCloseCallback(const CloseCallback& cb)
  { closeCallback_ = cb; }//关闭回调函数

  // called when TcpServer accepts a new connection连接建立
  void connectEstablished();   // should be called only once
  // called when TcpServer has removed me from its map
  void connectDestroyed();  // should be called only once连接销毁

 private:
  enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };//连接的四种状态
  void handleRead(Timestamp receiveTime);//将这些事件注册给channel去处理
  void handleWrite();
  void handleClose();
  void handleError();
  // void sendInLoop(string&& message);
  void sendInLoop(const StringPiece& message);
  void sendInLoop(const void* message, size_t len);
  void shutdownInLoop();
  // void shutdownAndForceCloseInLoop(double seconds);
  void forceCloseInLoop();
  void setState(StateE s) { state_ = s; }
  const char* stateToString() const;
  void startReadInLoop();
  void stopReadInLoop();

  EventLoop* loop_;
  const string name_;
  StateE state_;  // FIXME: use atomic variable
  // we don't expose those classes to client.
  boost::scoped_ptr<Socket> socket_;//持有惟一的sockfd
  boost::scoped_ptr<Channel> channel_;//持有Channel管理sockfd
  const InetAddress localAddr_;//客户与服务器的地址
  const InetAddress peerAddr_;
  ConnectionCallback connectionCallback_;//各种回调，应该是由TcpServer注册
  MessageCallback messageCallback_;
  WriteCompleteCallback writeCompleteCallback_;
  HighWaterMarkCallback highWaterMarkCallback_;
  CloseCallback closeCallback_;
  size_t highWaterMark_;
  Buffer inputBuffer_;
  Buffer outputBuffer_; // FIXME: use list<Buffer> as output buffer.
  boost::any context_;//？？
  bool reading_;//判断是否可读
  // FIXME: creationTime_, lastReceiveTime_
  //        bytesReceived_, bytesSent_
};

typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;

}
}

#endif  // MUDUO_NET_TCPCONNECTION_H
