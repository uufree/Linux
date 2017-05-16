// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_TIMERQUEUE_H
#define MUDUO_NET_TIMERQUEUE_H

#include <set>
#include <vector>

#include <boost/noncopyable.hpp>

#include <muduo/base/Mutex.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/Channel.h>

namespace muduo
{
namespace net
{

class EventLoop;
class Timer;
class TimerId;

///
/// A best efforts timer queue.
/// No guarantee that the callback will be on time.
///
class TimerQueue : boost::noncopyable
{
 public:
  TimerQueue(EventLoop* loop);
  ~TimerQueue();

  ///
  /// Schedules the callback to be run at given time,
  /// repeats if @c interval > 0.0.
  ///
  /// Must be thread safe. Usually be called from other threads.
  TimerId addTimer(const TimerCallback& cb,
                   Timestamp when,
                   double interval);//往定时器队列中添加定时器
#ifdef __GXX_EXPERIMENTAL_CXX0X__
  TimerId addTimer(TimerCallback&& cb,
                   Timestamp when,
                   double interval);
#endif

  void cancel(TimerId timerId);//取消某个定时器

 private:

  // FIXME: use unique_ptr<Timer> instead of raw pointers.
  typedef std::pair<Timestamp, Timer*> Entry;//到期的时间和指向其的定时器
  typedef std::set<Entry> TimerList;
  typedef std::pair<Timer*, int64_t> ActiveTimer;//定时器和其定时器的序列号
  typedef std::set<ActiveTimer> ActiveTimerSet;

  void addTimerInLoop(Timer* timer);
  void cancelInLoop(TimerId timerId);
  // called when timerfd alarms
  void handleRead();
  // move out all expired timers
  std::vector<Entry> getExpired(Timestamp now);//返回超时的定时器列表
  void reset(const std::vector<Entry>& expired, Timestamp now);

  bool insert(Timer* timer);//在两个序列中插入定时器

  EventLoop* loop_;
  const int timerfd_;//只有一个定时器，防止同时开启多个定时器，占用多余的文件描述符
  Channel timerfdChannel_;//定时器关心的channel对象
  // Timer list sorted by expiration
  TimerList timers_;//定时器集合（有序）

  // for cancel()
  // activeTimerSet和timer_保存的是相同的数据
  // timers_是按照到期的时间排序的，activeTimerSet_是按照对象地址排序
  ActiveTimerSet activeTimers_;//保存正在活动的定时器（无序）
  bool callingExpiredTimers_; /* atomic *///是否正在处理超时事件
  ActiveTimerSet cancelingTimers_;//保存的是取消的定时器（无序）
};

}
}
#endif  // MUDUO_NET_TIMERQUEUE_H
