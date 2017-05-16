// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_CHANNEL_H
#define MUDUO_NET_CHANNEL_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <muduo/base/Timestamp.h>

namespace muduo
{
namespace net
{

class EventLoop;

///
/// A selectable I/O channel.
///
/// This class doesn't own the file descriptor.
/// The file descriptor could be a socket,
/// an eventfd, a timerfd, or a signalfd
class Channel : boost::noncopyable
{
 public:
  typedef boost::function<void()> EventCallback;
  typedef boost::function<void(Timestamp)> ReadEventCallback;

  Channel(EventLoop* loop, int fd);
  ~Channel();

  void handleEvent(Timestamp receiveTime);//整体的事件处理函数
  void setReadCallback(const ReadEventCallback& cb)
  { readCallback_ = cb; }//各种事件注册函数，关键在于这些动作是由谁注册的
  void setWriteCallback(const EventCallback& cb)
  { writeCallback_ = cb; }
  void setCloseCallback(const EventCallback& cb)
  { closeCallback_ = cb; }
  void setErrorCallback(const EventCallback& cb)
  { errorCallback_ = cb; }
#ifdef __GXX_EXPERIMENTAL_CXX0X__
  void setReadCallback(ReadEventCallback&& cb)
  { readCallback_ = std::move(cb); }
  void setWriteCallback(EventCallback&& cb)
  { writeCallback_ = std::move(cb); }
  void setCloseCallback(EventCallback&& cb)
  { closeCallback_ = std::move(cb); }
  void setErrorCallback(EventCallback&& cb)
  { errorCallback_ = std::move(cb); }
#endif

  /// Tie this channel to the owner object managed by shared_ptr,
  /// prevent the owner object being destroyed in handleEvent.
  void tie(const boost::shared_ptr<void>&);

  int fd() const { return fd_; }//返回本channel处理的fd
  int events() const { return events_; }//返回关注的事件
  void set_revents(int revt) { revents_ = revt; } //设置现在发生的事件，在新创建一个channel的时候调用
  // int revents() const { return revents_; }
  bool isNoneEvent() const { return events_ == kNoneEvent; }//什么事件都不关注

  void enableReading() { events_ |= kReadEvent; update(); }//将关注的事件变为可读，然后更新
  void disableReading() { events_ &= ~kReadEvent; update(); }
  void enableWriting() { events_ |= kWriteEvent; update(); }
  void disableWriting() { events_ &= ~kWriteEvent; update(); }
  void disableAll() { events_ = kNoneEvent; update(); }
  bool isWriting() const { return events_ & kWriteEvent; }//是否可写
  bool isReading() const { return events_ & kReadEvent; }//是否可读

  // for Poller
  int index() { return index_; }//返回在POLLER中的索引
  void set_index(int idx) { index_ = idx; }//设置在POLLER中的索引

  // for debug
  string reventsToString() const;
  string eventsToString() const;

  void doNotLogHup() { logHup_ = false; }

  EventLoop* ownerLoop() { return loop_; }//返回所在的LOOP
  void remove();

 private:
  static string eventsToString(int fd, int ev);//日志操作

  void update();//更新事件
  void handleEventWithGuard(Timestamp receiveTime);//？？

  static const int kNoneEvent;//三种事件状态
  static const int kReadEvent;
  static const int kWriteEvent;

  EventLoop* loop_;//所在的Loop
  const int  fd_;//channel锁关注的sockfd上发生的事件
  int        events_;//本channel关注的事件
  int        revents_; //目前本channel发生的事件
  int        index_; //在pollfd中的索引
  bool       logHup_;//是否添加日志

  boost::weak_ptr<void> tie_;//指向connection
  bool tied_;//是否有指向
  bool eventHandling_;//事件处理状态
  bool addedToLoop_;//判断这个channel是否已经被添加到Loop中
  ReadEventCallback readCallback_;//回调函数
  EventCallback writeCallback_;
  EventCallback closeCallback_;
  EventCallback errorCallback_;
};

}
}
#endif  // MUDUO_NET_CHANNEL_H
