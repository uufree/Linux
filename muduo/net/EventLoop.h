// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include <vector>

#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <muduo/base/Mutex.h>
#include <muduo/base/CurrentThread.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/TimerId.h>

namespace muduo
{
namespace net
{

class Channel;
class Poller;
class TimerQueue;

///
/// Reactor, at most one per thread.
///
/// This is an interface class, so don't expose too much details.
class EventLoop : boost::noncopyable
{
 public:
  typedef boost::function<void()> Functor;//用于处理其他线程注册给loop线程的事件

  EventLoop();
  ~EventLoop();  // force out-line dtor, for scoped_ptr members.

  ///
  /// Loops forever.
  ///
  /// Must be called in the same thread as creation of the object.
  ///
  void loop();//开始循环

  /// Quits loop.
  ///
  /// This is not 100% thread safe, if you call through a raw pointer,
  /// better to call through shared_ptr<EventLoop> for 100% safety.
  void quit();//循环停止

  ///
  /// Time when poll returns, usually means data arrival.
  ///
  Timestamp pollReturnTime() const { return pollReturnTime_; }//获得Pooler等待的事件

  int64_t iteration() const { return iteration_; }//获得总共的Poll次数

  /// Runs callback immediately in the loop thread.
  /// It wakes up the loop, and run the cb.
  /// If in the same loop thread, cb is run within the function.
  /// Safe to call from other threads.
  void runInLoop(const Functor& cb);//其他线程向loop线程注册事件
  /// Queues callback in the loop thread.
  /// Runs after finish pooling.
  /// Safe to call from other threads.
  void queueInLoop(const Functor& cb);//将其他线程的事件放到loop线程的待处理队列中

  size_t queueSize() const;//队列长度

#ifdef __GXX_EXPERIMENTAL_CXX0X__
  void runInLoop(Functor&& cb);
  void queueInLoop(Functor&& cb);
#endif

  // timers

  ///
  /// Runs callback at 'time'.
  /// Safe to call from other threads.
  ///
  TimerId runAt(const Timestamp& time, const TimerCallback& cb);//三种定时事件
  ///
  /// Runs callback after @c delay seconds.
  /// Safe to call from other threads.
  ///
  TimerId runAfter(double delay, const TimerCallback& cb);
  ///
  /// Runs callback every @c interval seconds.
  /// Safe to call from other threads.
  ///
  TimerId runEvery(double interval, const TimerCallback& cb);
  ///
  /// Cancels the timer.
  /// Safe to call from other threads.
  ///
  void cancel(TimerId timerId);//取消某个定时器

#ifdef __GXX_EXPERIMENTAL_CXX0X__
  TimerId runAt(const Timestamp& time, TimerCallback&& cb);
  TimerId runAfter(double delay, TimerCallback&& cb);
  TimerId runEvery(double interval, TimerCallback&& cb);
#endif

  // internal usage
  void wakeup();//用于将线程的控制流从poll的阻塞状态唤醒
  void updateChannel(Channel* channel);//更新事件
  void removeChannel(Channel* channel);//移除事件
  bool hasChannel(Channel* channel);//判断是否本Loop是否含有这个channel

  // pid_t threadId() const { return threadId_; }
  void assertInLoopThread()
  {
    if (!isInLoopThread())//如果不在loop线程，直接异常退出
    {
      abortNotInLoopThread();
    }
  }
  bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }//判断是否在Loop线程
  // bool callingPendingFunctors() const { return callingPendingFunctors_; }
  bool eventHandling() const { return eventHandling_; }//判断是否在事件处理状态

  void setContext(const boost::any& context)
  { context_ = context; }//？？

  const boost::any& getContext() const
  { return context_; }//？？

  boost::any* getMutableContext()
  { return &context_; }//？？

  static EventLoop* getEventLoopOfCurrentThread();//获得当前线程的loop

 private:
  void abortNotInLoopThread();//终止不再loop线程的调用
  void handleRead();  //唤醒处于poll的控制流
  void doPendingFunctors();//处理其他线程注册给Loop的函数

  void printActiveChannels() const; // DEBUG

  typedef std::vector<Channel*> ChannelList;

  bool looping_; /* atomic *///正在进行LOOP循环
  bool quit_; /* atomic and shared between threads, okay on x86, I guess. *///是否已经停止
  bool eventHandling_; /* atomic *///事件处理状态
  bool callingPendingFunctors_; /* atomic *///注册事件处理状态
  int64_t iteration_;//表示总共的POLL次数
  const pid_t threadId_;//唯一的线程标识
  Timestamp pollReturnTime_;
  boost::scoped_ptr<Poller> poller_;//唯一的poller
  boost::scoped_ptr<TimerQueue> timerQueue_;//唯一的TimeQueue
  int wakeupFd_;//唤醒描述符
  // unlike in TimerQueue, which is an internal class,
  // we don't expose Channel to client.
  boost::scoped_ptr<Channel> wakeupChannel_;//唤醒描述符的事件分发表
  boost::any context_;//？？

  // scratch variables
  ChannelList activeChannels_;//活动的事件集合，每次使用前需要清空
  Channel* currentActiveChannel_;//当前正在处理的事件

  mutable MutexLock mutex_;//由于是其他线程向loop线程写事件，所以需要MUTEX
  std::vector<Functor> pendingFunctors_; //loop线程注册事件的集合
};

}
}
#endif  // MUDUO_NET_EVENTLOOP_H
