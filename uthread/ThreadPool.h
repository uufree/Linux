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
#include<deque>
#include<memory>

namespace thread
{
    class ThreadPool final
    {
        typedef std::function<void()> ThreadFunc;
        typedef std::deque<ThreadFunc> TaskQueue;

        public:
            explicit ThreadPool(int size = 2) : 
                started(false),
                threadsize(size),
                threadlistptr(new Thread[size]),
                mutex(),
                cond(mutex)
            {};

            explicit ThreadPool(int size,const ThreadFunc& cb) : 
                started(false),
                threadsize(size),
                threadlistptr(new Thread[size]),
                threadfunc(cb),
                mutex(),
                cond(mutex)
            {};
                
            ThreadPool(const ThreadPool& lhs);
            ThreadPool(ThreadPool&& lhs);
            ThreadPool& operator=(const ThreadPool& lhs);
                
            ~ThreadPool();

//public interface
            void setThreadCallBack(const ThreadFunc& cb)
            {
                if(!started)
                    threadfunc = cb;
            }

            void start();
            void joinAll();
            void addInTaskQueue(const ThreadFunc& task);
            ThreadFunc getTaskInTaskQueue();

        private:
            bool started;
            const int threadsize;
            Thread* threadlistptr;
            ThreadFunc threadfunc;
            TaskQueue taskqueue;
            MutexLock mutex;
            Condition cond;
    };
}

#endif

