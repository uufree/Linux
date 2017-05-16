// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include <muduo/base/ThreadPool.h>

#include <muduo/base/Exception.h>

#include <boost/bind.hpp>
#include <assert.h>
#include <stdio.h>

using namespace muduo;

ThreadPool::ThreadPool(const string& nameArg)
  : mutex_(),
    notEmpty_(mutex_),
    notFull_(mutex_),
    name_(nameArg),
    maxQueueSize_(0),
    running_(false)
{
}

ThreadPool::~ThreadPool()
{
  if (running_)
  {
    stop();
  }
}

void ThreadPool::start(int numThreads)
{
  assert(threads_.empty());//首次启动，断言线程池为空
  running_ = true;
  threads_.reserve(numThreads);//预分配空间
  for (int i = 0; i < numThreads; ++i)
  {
    char id[32];
    snprintf(id, sizeof id, "%d", i+1);
    threads_.push_back(new muduo::Thread(
          boost::bind(&ThreadPool::runInThread, this), name_+id));
    threads_[i].start();//直接启动线程
  }
  if (numThreads == 0 && threadInitCallback_)//只启动一条线程
  {
    threadInitCallback_();
  }
}

void ThreadPool::stop()
{
  {
  MutexLockGuard lock(mutex_);
  running_ = false;
  notEmpty_.notifyAll();//目的在于通知notempty条件变量
  }
  for_each(threads_.begin(),
           threads_.end(),
           boost::bind(&muduo::Thread::join, _1));//使用STL算法依次关闭线程池中运行的线程
}

size_t ThreadPool::queueSize() const
{//获得排队任务执行队列的队列长度
  MutexLockGuard lock(mutex_);
  return queue_.size();
}

void ThreadPool::run(const Task& task)
{
  if (threads_.empty())//如果线程池为空，直接跑这条线程
  {
    task();
  }
  else
  {
    MutexLockGuard lock(mutex_);
    while (isFull())//如果线程等待队列满了，在notfull条件变量上等待
    {
      notFull_.wait();
    }
    assert(!isFull());

    queue_.push_back(task);//现在线程池执行任务队列中有空位了
    notEmpty_.notify();//notempty条件变量通知信息
  }
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
void ThreadPool::run(Task&& task)
{
  if (threads_.empty())
  {
    task();
  }
  else
  {
    MutexLockGuard lock(mutex_);
    while (isFull())
    {
      notFull_.wait();
    }
    assert(!isFull());

    queue_.push_back(std::move(task));
    notEmpty_.notify();
  }
}
#endif

ThreadPool::Task ThreadPool::take()
{
  MutexLockGuard lock(mutex_);
  // always use a while-loop, due to spurious wakeup
  while (queue_.empty() && running_)//如果线程队列为空并且线程池正在跑
  {//在notempty条件变量上等待
    notEmpty_.wait();//当前线程停下来等待，当队列不为空了继续跑
  }//然后获得新任务
  Task task;//创建一个新的任务
  if (!queue_.empty())
  {
    task = queue_.front();//获得队列中的头任务
    queue_.pop_front();//弹出队列中的头任务
    if (maxQueueSize_ > 0)//如果队列最大长度大于0
    {
      notFull_.notify();//通知线程可以跑了
    }
  }
  return task;//返回任务
}

bool ThreadPool::isFull() const
{//用来判断线程队列是否已经满了
  mutex_.assertLocked();
  return maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_;
}

void ThreadPool::runInThread()//生成一个threadFunc对象
{
  try
  {
    if (threadInitCallback_)//如果线程启动函数不为空，直接启动
    {
      threadInitCallback_();
    }
    while (running_)//while循环保证线程任务等待队列中没有多余的任务
    {
      Task task(take());
      if (task)
      {
        task();
      }
    }
  }
  catch (const Exception& ex)   //异常捕捉过程
  {
    fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
    fprintf(stderr, "reason: %s\n", ex.what());
    fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
    abort();
  }
  catch (const std::exception& ex)
  {
    fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
    fprintf(stderr, "reason: %s\n", ex.what());
    abort();
  }
  catch (...)
  {
    fprintf(stderr, "unknown exception caught in ThreadPool %s\n", name_.c_str());
    throw; // rethrow
  }
}

