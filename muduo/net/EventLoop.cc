// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include <muduo/net/EventLoop.h>

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/Channel.h>
#include <muduo/net/Poller.h>
#include <muduo/net/SocketsOps.h>
#include <muduo/net/TimerQueue.h>

#include <boost/bind.hpp>

#include <signal.h>
#include <sys/eventfd.h>

using namespace muduo;
using namespace muduo::net;

namespace
{
__thread EventLoop* t_loopInThisThread = 0;//确保每个线程在一段事件内只有一个Loop对象

const int kPollTimeMs = 10000;//每次POLLER阻塞的事件

int createEventfd()
{//创建一个POLLER事件描述符
  int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evtfd < 0)
  {
    LOG_SYSERR << "Failed in eventfd";
    abort();
  }
  return evtfd;
}

#pragma GCC diagnostic ignored "-Wold-style-cast"
class IgnoreSigPipe
{//阻塞某个特定的信号
 public:
  IgnoreSigPipe()
  {
    ::signal(SIGPIPE, SIG_IGN);
    // LOG_TRACE << "Ignore SIGPIPE";
  }
};
#pragma GCC diagnostic error "-Wold-style-cast"

IgnoreSigPipe initObj;//以全局对象的角度阻塞某个信号
}

EventLoop* EventLoop::getEventLoopOfCurrentThread()
{//返回当前线程的事件注册表
  return t_loopInThisThread;
}

EventLoop::EventLoop()
  : looping_(false),
    quit_(false),
    eventHandling_(false),
    callingPendingFunctors_(false),
    iteration_(0),
    threadId_(CurrentThread::tid()),
    poller_(Poller::newDefaultPoller(this)),
    timerQueue_(new TimerQueue(this)),
    wakeupFd_(createEventfd()),
    wakeupChannel_(new Channel(this, wakeupFd_)),
    currentActiveChannel_(NULL)
{
  LOG_DEBUG << "EventLoop created " << this << " in thread " << threadId_;
  if (t_loopInThisThread)//判断当前线程是否有LOOP
  {
    LOG_FATAL << "Another EventLoop " << t_loopInThisThread
              << " exists in this thread " << threadId_;
  }
  else
  {
    t_loopInThisThread = this;//设置当前线程的LOOP。__thread
  }
  wakeupChannel_->setReadCallback(
      boost::bind(&EventLoop::handleRead, this));//向本loop的weakup描述符注册可读事件
  // we are always reading the wakeupfd
  wakeupChannel_->enableReading();//将weakup_channel设置为关注可读事件
}

EventLoop::~EventLoop()
{
  LOG_DEBUG << "EventLoop " << this << " of thread " << threadId_
            << " destructs in thread " << CurrentThread::tid();
  wakeupChannel_->disableAll();//本channel不再关注任何时间
  wakeupChannel_->remove();//移除Channel
  ::close(wakeupFd_);//关闭紧急唤醒描述符
  t_loopInThisThread = NULL;//说明当前线程没有LOOP
}

void EventLoop::loop()
{
  assert(!looping_);//判断是否在LOOPING
  assertInLoopThread();//判断这个函数在LOOP线程调用
  looping_ = true;//进入LOOPING状态
  quit_ = false;  // FIXME: what if someone calls quit() before loop() ?
  LOG_TRACE << "EventLoop " << this << " start looping";

  while (!quit_)
  {
    activeChannels_.clear();//将活动线程队列置空
    pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);//获得活动文件描述符的数量，并且获得活动的channel队列
    ++iteration_;//增加Poll次数
    if (Logger::logLevel() <= Logger::TRACE)
    {
      printActiveChannels();
    }
    // TODO sort channel by priority
    eventHandling_ = true;//事件处理状态
    for (ChannelList::iterator it = activeChannels_.begin();
        it != activeChannels_.end(); ++it)
    {
      currentActiveChannel_ = *it;//获得当前活动的事件
      currentActiveChannel_->handleEvent(pollReturnTime_);//处理事件，传递一个poll的阻塞时间
    }
    currentActiveChannel_ = NULL;//将当前活动事件置为空
    eventHandling_ = false;//退出事件处理状态
    doPendingFunctors();//处理用户在其他线程注册给IO线程的事件
  }

  LOG_TRACE << "EventLoop " << this << " stop looping";
  looping_ = false;//推出LOOPING状态
}

void EventLoop::quit()
{
  quit_ = true;
  // There is a chance that loop() just executes while(!quit_) and exits,
  // then EventLoop destructs, then we are accessing an invalid object.
  // Can be fixed using mutex_ in both places.
  if (!isInLoopThread())//判断是否在loop线程，如果不再loop线程则唤醒处于LOOPING状态LOOP
  {
    wakeup();
  }
}

void EventLoop::runInLoop(const Functor& cb)//其他线程将事件注册给loop线程
{
  if (isInLoopThread())//如果在loop线程，直接运行
  {
    cb();
  }
  else
  {
    queueInLoop(cb);//如果不在，将事件添加到注册事件队列中
  }
}

void EventLoop::queueInLoop(const Functor& cb)
{
  {
  MutexLockGuard lock(mutex_);
  pendingFunctors_.push_back(cb);
  }

  if (!isInLoopThread() || callingPendingFunctors_)//如果没有在IO线程或者没有正在处理其他线程的注册时间，唤醒线程，此处的唤醒是指让LOOP线程退出POLLER的阻塞等待状态
  {
    wakeup();
  }
}

size_t EventLoop::queueSize() const//返回第三方队列的长度
{
  MutexLockGuard lock(mutex_);
  return pendingFunctors_.size();
}

TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
{
  return timerQueue_->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback& cb)
{
  Timestamp time(addTime(Timestamp::now(), delay));
  return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback& cb)
{
  Timestamp time(addTime(Timestamp::now(), interval));
  return timerQueue_->addTimer(cb, time, interval);
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
// FIXME: remove duplication
void EventLoop::runInLoop(Functor&& cb)
{
  if (isInLoopThread())
  {
    cb();
  }
  else
  {
    queueInLoop(std::move(cb));
  }
}

void EventLoop::queueInLoop(Functor&& cb)
{
  {
  MutexLockGuard lock(mutex_);
  pendingFunctors_.push_back(std::move(cb));  // emplace_back
  }

  if (!isInLoopThread() || callingPendingFunctors_)
  {
    wakeup();
  }
}

TimerId EventLoop::runAt(const Timestamp& time, TimerCallback&& cb)
{
  return timerQueue_->addTimer(std::move(cb), time, 0.0);
}

TimerId EventLoop::runAfter(double delay, TimerCallback&& cb)
{
  Timestamp time(addTime(Timestamp::now(), delay));
  return runAt(time, std::move(cb));
}

TimerId EventLoop::runEvery(double interval, TimerCallback&& cb)
{
  Timestamp time(addTime(Timestamp::now(), interval));
  return timerQueue_->addTimer(std::move(cb), time, interval);
}
#endif

void EventLoop::cancel(TimerId timerId)
{
  return timerQueue_->cancel(timerId);
}

void EventLoop::updateChannel(Channel* channel)
{
  assert(channel->ownerLoop() == this);//判断channel的LOOP是否是当前的LOOP
  assertInLoopThread();//判断是否在IO线程
  poller_->updateChannel(channel);//使用POLLER来更新channel
}

void EventLoop::removeChannel(Channel* channel)
{//每次间接的调用的作用就是将需要改动的东西与当前调用的类撇清关系
  assert(channel->ownerLoop() == this);
  assertInLoopThread();//如果没有在loop线程调用直接退出
  if (eventHandling_)//判断是否在事件处理状态。判断当前是否在处理这个将要删除的事件以及活动的事件表中是否有这个事件
  {
    assert(currentActiveChannel_ == channel ||
        std::find(activeChannels_.begin(), activeChannels_.end(), channel) == activeChannels_.end());
  }
  poller_->removeChannel(channel);//在POLLER中删除这个事件分发表
}

bool EventLoop::hasChannel(Channel* channel)//判断这个channel是否在LOOP中
{
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  return poller_->hasChannel(channel);
}

void EventLoop::abortNotInLoopThread()
{
  LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
            << " was created in threadId_ = " << threadId_
            << ", current thread id = " <<  CurrentThread::tid();
}

void EventLoop::wakeup()//具体的唤醒步骤
{
  uint64_t one = 1;
  ssize_t n = sockets::write(wakeupFd_, &one, sizeof one);
  if (n != sizeof one)
  {
    LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
  }
}

void EventLoop::handleRead()
{
  uint64_t one = 1;
  ssize_t n = sockets::read(wakeupFd_, &one, sizeof one);
  if (n != sizeof one)
  {
    LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
  }
}

void EventLoop::doPendingFunctors()
{//处理其他线程向本线程注册的事件
  std::vector<Functor> functors;
  callingPendingFunctors_ = true;//处于处理外来事件的状态

  {
  MutexLockGuard lock(mutex_);
  functors.swap(pendingFunctors_);
  }

  for (size_t i = 0; i < functors.size(); ++i)
  {
    functors[i]();
  }
  callingPendingFunctors_ = false;
}

void EventLoop::printActiveChannels() const
{
  for (ChannelList::const_iterator it = activeChannels_.begin();
      it != activeChannels_.end(); ++it)
  {
    const Channel* ch = *it;
    LOG_TRACE << "{" << ch->reventsToString() << "} ";
  }
}

