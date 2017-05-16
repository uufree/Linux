// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_TIMER_H
#define MUDUO_NET_TIMER_H

#include <boost/noncopyable.hpp>

#include <muduo/base/Atomic.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>

namespace muduo
{
namespace net
{
///
/// Internal class for timer event.
//表示计时器的类
class Timer : boost::noncopyable
{
 public:
  Timer(const TimerCallback& cb, Timestamp when, double interval)
    : callback_(cb),
      expiration_(when),
      interval_(interval),
      repeat_(interval > 0.0),
      sequence_(s_numCreated_.incrementAndGet())
  { }

#ifdef __GXX_EXPERIMENTAL_CXX0X__
  Timer(TimerCallback&& cb, Timestamp when, double interval)
    : callback_(std::move(cb)),
      expiration_(when),
      interval_(interval),
      repeat_(interval > 0.0),
      sequence_(s_numCreated_.incrementAndGet())
  { }
#endif

  void run() const
  {//跑的回调函数
    callback_();
  }

  Timestamp expiration() const  { return expiration_; }//返回的当前的时间
  bool repeat() const { return repeat_; }//判断是否有重复的属性
  int64_t sequence() const { return sequence_; }//返回的队列

  void restart(Timestamp now);//重新设置到时时间

  static int64_t numCreated() { return s_numCreated_.get(); }

 private:
  const TimerCallback callback_;//定时器回调函数
  Timestamp expiration_;//绝对的时间
  const double interval_;//如果有重复属性，超时的时间间隔
  const bool repeat_;//是否有重复
  const int64_t sequence_;//定时器序号

  static AtomicInt64 s_numCreated_;//定时器计数
};
}
}
#endif  // MUDUO_NET_TIMER_H
