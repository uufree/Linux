// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_THREAD_H
#define MUDUO_BASE_THREAD_H

#include <muduo/base/Atomic.h>//在Thread类中使用AtomicInt32
#include <muduo/base/Types.h>//不知道哎

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>

namespace muduo
{

class Thread : boost::noncopyable   //禁止拷贝
{
 public:
  typedef boost::function<void ()> ThreadFunc;//仿函数对象,利用回调的方式使用线程函数

  explicit Thread(const ThreadFunc&, const string& name = string());//普通的线程构造函数
#ifdef __GXX_EXPERIMENTAL_CXX0X__
  explicit Thread(ThreadFunc&&, const string& name = string());//移动的线程构造函数，比上面的更节省资源std::move
#endif
  ~Thread();//析构函数

  void start();//启动线程
  int join(); // 类似于 pthread_join()

  bool started() const { return started_; }
  // pthread_t pthreadId() const { return pthreadId_; }
  pid_t tid() const { return *tid_; } //返回线程索引
  const string& name() const { return name_; }//返回线程名字

  static int numCreated() { return numCreated_.get(); }

 private:
  void setDefaultName();

  bool       started_;  //是否启动
  bool       joined_;   //是否终止
  pthread_t  pthreadId_;    //线程索引
  boost::shared_ptr<pid_t> tid_;    //指向线程索引的智能指针
  ThreadFunc func_;     //线程主题函数
  string     name_;     //线程名字

  static AtomicInt32 numCreated_;   //static变量在所有的线程对象中共享，为由该类产生的线程排序
};

}
#endif
