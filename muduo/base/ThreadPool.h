// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_THREADPOOL_H
#define MUDUO_BASE_THREADPOOL_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Types.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <deque>

namespace muduo
{

//线程池的核心观念是线程是基本的存在单位

class ThreadPool : boost::noncopyable
{
 public:
  typedef boost::function<void ()> Task;//将线程池中的线程作为可替换的任务，以线程为基本单位放在线程池中运行

  explicit ThreadPool(const string& nameArg = string("ThreadPool"));
  ~ThreadPool();

  // Must be called before start().
  // 设置线程池运行的最大的负载以及线程池中将要运行的线程
  void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }//
  void setThreadInitCallback(const Task& cb)
  { threadInitCallback_ = cb; }

  void start(int numThreads);//启动一定数量的线程
  void stop();//线程池运算停止

  const string& name() const
  { return name_; }

  size_t queueSize() const;//返回正在排队等待的线程任务

  // Could block if maxQueueSize > 0
  void run(const Task& f);//将一个想要运行的线程放入线程池的任务队列
#ifdef __GXX_EXPERIMENTAL_CXX0X__
  void run(Task&& f);//C++11的移动方法，用于节省资源
#endif

 private:
  bool isFull() const;//判断线程队列是否已经满了
  void runInThread();//真正让线程跑起来的函数
  Task take();//获得任务队列的首个线程

  mutable MutexLock mutex_;//互斥锁
  Condition notEmpty_;//条件变量
  Condition notFull_;
  string name_;
  Task threadInitCallback_;//线程池中执行的线程对象
  boost::ptr_vector<muduo::Thread> threads_;//线程池
  std::deque<Task> queue_;//排队执行的线程对象队列
  size_t maxQueueSize_;//等待队列的最大数
  bool running_;//是否已经启动
};

}

#endif
