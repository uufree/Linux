/*************************************************************************
	> File Name: ThreadPool.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月27日 星期一 23时19分53秒
 ************************************************************************/

#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include"Thread.h"
#include"Mutex.h"
#include"Condition.h"
#include<map>
#include<deque>

namespace thread
{
    class ThreadPool final
    {
        typedef std::function<void()> ThreadFunc;
        typedef std::map<pthread_t,Thread> ThreadMap;
        typedef std::deque<ThreadFunc> TaskQueue;

        public:
            explicit ThreadPool(int size = 2) : 
                threadsize(size),
                mutex(),
                cond(mutex)
            {};

            explicit ThreadPool(int size,const ThreadFunc& cb) : 
                threadsize(size),
                threadfunc(cb),
                mutex(),
                cond(mutex)
            {};
                
            ThreadPool(const ThreadPool& lhs) = delete;
            ThreadPool(ThreadPool&& lhs) = delete;
            ThreadPool& operator=(const ThreadPool& lhs) = delete;
                
            ~ThreadPool();

//public interface
            void setThreadCallBack(const ThreadFunc& cb)
            {
                threadfunc = cb;
            }

            void start();
            void joinAll();
            void addInTaskQueue(const ThreadFunc& task);
            ThreadFunc&& getTaskInTaskQueue();

        private:
            const int threadsize;
            ThreadMap threadmap;
            ThreadFunc threadfunc;
            TaskQueue taskqueue;
            MutexLock mutex;
            Condition cond;
    };
}

#endif

