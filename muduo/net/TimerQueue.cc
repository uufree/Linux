// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include <muduo/net/TimerQueue.h>

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/Timer.h>
#include <muduo/net/TimerId.h>

#include <boost/bind.hpp>

#include <sys/timerfd.h>

namespace muduo
{
namespace net
{
namespace detail
{

int createTimerfd()
{//创建非阻塞timefd
  int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                 TFD_NONBLOCK | TFD_CLOEXEC);
  if (timerfd < 0)
  {
    LOG_SYSFATAL << "Failed in timerfd_create";
  }
  return timerfd;
}

struct timespec howMuchTimeFromNow(Timestamp when)
{//现在距离超时还有多久
  int64_t microseconds = when.microSecondsSinceEpoch()
                         - Timestamp::now().microSecondsSinceEpoch();
  if (microseconds < 100)
  {
    microseconds = 100;
  }
  struct timespec ts;
  ts.tv_sec = static_cast<time_t>(
      microseconds / Timestamp::kMicroSecondsPerSecond);
  ts.tv_nsec = static_cast<long>(
      (microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
  return ts;
}

void readTimerfd(int timerfd, Timestamp now)
{//处理超时时间，超时后，timefd变为可读,howmany表示超时的次数
  uint64_t howmany;//将事件读出来，免得陷入Loop忙碌状态
  ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
  LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
  if (n != sizeof howmany)
  {
    LOG_ERROR << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
  }
}

void resetTimerfd(int timerfd, Timestamp expiration)
{//重新设置定时器描述符关注的定时事件
  // wake up loop by timerfd_settime()
  struct itimerspec newValue;
  struct itimerspec oldValue;
  bzero(&newValue, sizeof newValue);
  bzero(&oldValue, sizeof oldValue);
  newValue.it_value = howMuchTimeFromNow(expiration);//获得与现在的时间差值，然后设置关注事件
  int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
  if (ret)
  {
    LOG_SYSERR << "timerfd_settime()";
  }
}

}
}
}

using namespace muduo;
using namespace muduo::net;
using namespace muduo::net::detail;

TimerQueue::TimerQueue(EventLoop* loop)
  : loop_(loop),
    timerfd_(createTimerfd()),
    timerfdChannel_(loop, timerfd_),
    timers_(),
    callingExpiredTimers_(false)
{
  timerfdChannel_.setReadCallback(
      boost::bind(&TimerQueue::handleRead, this));
  // we are always reading the timerfd, we disarm it with timerfd_settime.
  timerfdChannel_.enableReading();//设置Channel的常规步骤
}

TimerQueue::~TimerQueue()
{
  timerfdChannel_.disableAll();//channel不再关注任何事件
  timerfdChannel_.remove();//在三角循环中删除此Channel
  ::close(timerfd_);
  // do not remove channel, since we're in EventLoop::dtor();
  for (TimerList::iterator it = timers_.begin();
      it != timers_.end(); ++it)
  {
    delete it->second;//释放timer对象
  }
}

TimerId TimerQueue::addTimer(const TimerCallback& cb,
                             Timestamp when,
                             double interval)
{//添加新的定时器
  Timer* timer = new Timer(cb, when, interval);
  loop_->runInLoop(
      boost::bind(&TimerQueue::addTimerInLoop, this, timer));
  return TimerId(timer, timer->sequence());
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
TimerId TimerQueue::addTimer(TimerCallback&& cb,
                             Timestamp when,
                             double interval)
{
  Timer* timer = new Timer(std::move(cb), when, interval);
  loop_->runInLoop(
      boost::bind(&TimerQueue::addTimerInLoop, this, timer));
  return TimerId(timer, timer->sequence());
}
#endif

void TimerQueue::cancel(TimerId timerId)
{//取消定时器
  loop_->runInLoop(
      boost::bind(&TimerQueue::cancelInLoop, this, timerId));
}

void TimerQueue::addTimerInLoop(Timer* timer)
{
  loop_->assertInLoopThread();
  bool earliestChanged = insert(timer);//是否将timer插入set的首部

  //如果插入首部，更新timrfd关注的到期时间
  if (earliestChanged)
  {
    resetTimerfd(timerfd_, timer->expiration());//启动定时器
  }
}

void TimerQueue::cancelInLoop(TimerId timerId)
{//取消要关注的重复事件
  loop_->assertInLoopThread();
  assert(timers_.size() == activeTimers_.size());
  ActiveTimer timer(timerId.timer_, timerId.sequence_);//获得索引
  ActiveTimerSet::iterator it = activeTimers_.find(timer);
  if (it != activeTimers_.end())
  {//删除Timers_和activeTimers_中的Timer
    size_t n = timers_.erase(Entry(it->first->expiration(), it->first));
    assert(n == 1); (void)n;
    delete it->first; // FIXME: no delete please
    activeTimers_.erase(it);//删除活动的timer
  }
  else if (callingExpiredTimers_)
  {//将删除的timer加入到取消的timer队列中
    cancelingTimers_.insert(timer);//取消的定时器与重新启动定时器有冲突
  }
  assert(timers_.size() == activeTimers_.size());
}

void TimerQueue::handleRead()
{
  loop_->assertInLoopThread();
  Timestamp now(Timestamp::now());
  readTimerfd(timerfd_, now);//读timerFd,防止一直出现可读事件，造成loop忙碌

  std::vector<Entry> expired = getExpired(now);//获得超时的定时器

  callingExpiredTimers_ = true;//将目前的状态调整为处理超时状态
  cancelingTimers_.clear();//将取消的定时器清理掉
  //更新完成马上就是重置，重置时依赖已经取消的定时器的条件，所以要将取消的定时器的队列清空
  // safe to callback outside critical section
  for (std::vector<Entry>::iterator it = expired.begin();
      it != expired.end(); ++it)//逐个调用超时的定时器的回调
  {
    it->second->run();
  }
  callingExpiredTimers_ = false;//退出处理超时定时器额状态

  reset(expired, now);//把具有重复属性的定时器重新加入定时器队列中
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{//获得当前已经超时的timer
  assert(timers_.size() == activeTimers_.size());
  std::vector<Entry> expired;//存储超时timer的队列
  Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
  TimerList::iterator end = timers_.lower_bound(sentry);//返回的一个大于等于now的timer，小于now的都已经超时
  assert(end == timers_.end() || now < end->first);
  std::copy(timers_.begin(), end, back_inserter(expired));//将timer_的begin到上述获得end迭代器元素添加到expired的末尾
  timers_.erase(timers_.begin(), end);//在timer_中删除刚才被添加的元素

  for (std::vector<Entry>::iterator it = expired.begin();
      it != expired.end(); ++it)
  {//在Activetimer_的同步中删除timer
    ActiveTimer timer(it->second, it->second->sequence());
    size_t n = activeTimers_.erase(timer);
    assert(n == 1); (void)n;
  }

  assert(timers_.size() == activeTimers_.size());//再次将timer_和activetimer同步
  return expired;//返回超时的timerQueue
}

void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{//将具有超时属性的定时器重新加入定时器队列
  Timestamp nextExpire;

  for (std::vector<Entry>::const_iterator it = expired.begin();
      it != expired.end(); ++it)
  {
    ActiveTimer timer(it->second, it->second->sequence());
    if (it->second->repeat()
        && cancelingTimers_.find(timer) == cancelingTimers_.end())
    {//判断是否具有重复属性并且不在取消的定时器队列中
      it->second->restart(now);//重新设置定时器的到期时间，并且将重新设置后的定时器插入timer_和activeTimer_中
      insert(it->second);
    }
    else
    {
      // FIXME move to a free list
      delete it->second; // FIXME: no delete please
    }
  }

  if (!timers_.empty())
  {//如果目前的队列不为空，获得目前队首的到期时间
    nextExpire = timers_.begin()->second->expiration();
  }

  if (nextExpire.valid())
  {//如果到期时间不为0,重新设置timerfd应该关注的时间
    resetTimerfd(timerfd_, nextExpire);
  }
}

bool TimerQueue::insert(Timer* timer)
{//将Timer插入到两个同步的TimeQueue中，最关键的一个函数
  loop_->assertInLoopThread();
  assert(timers_.size() == activeTimers_.size());//判断两个Timer队列的同步bool earliestChanged = false;
  Timestamp when = timer->expiration();//获得Timer的事件
  TimerList::iterator it = timers_.begin();//得到Timer的begin
  if (it == timers_.end() || when < it->first)
  {//判断是否要将这个timer插入队首，如果是，更新timefd关注的到期事件
    earliestChanged = true;
  }

  {//将Timer中按顺序插入timer_，set是有序集合，默认关键字<排列
    std::pair<TimerList::iterator, bool> result
      = timers_.insert(Entry(when, timer));
    assert(result.second); (void)result;
  }

  {//随意插入进入activeTimer_
    std::pair<ActiveTimerSet::iterator, bool> result
      = activeTimers_.insert(ActiveTimer(timer, timer->sequence()));
    assert(result.second); (void)result;
  }

  assert(timers_.size() == activeTimers_.size());//再次同步两个Timer
  return earliestChanged;
}

