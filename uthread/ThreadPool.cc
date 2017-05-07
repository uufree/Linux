/*************************************************************************
	> File Name: ThreadPool.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月27日 星期一 23时31分49秒
 ************************************************************************/

#include"ThreadPool.h"

namespace thread
{
    ThreadPool::~ThreadPool()
    {
        started = false;
        for(int i=0;i<threadsize;++i)
            ::pthread_detach(threadlistptr[i].getThreadId());

        delete [] threadlistptr;
    }
    
    ThreadPool::ThreadPool(const ThreadPool& lhs) : 
        started(false),
        threadsize(lhs.threadsize),
        threadlistptr(new Thread[threadsize]),
        threadfunc(lhs.threadfunc),
        mutex(),
        cond(mutex)
    {};
    
    ThreadPool::ThreadPool(ThreadPool&& lhs) : 
        started(false),
        threadsize(lhs.threadsize),
        threadlistptr(new Thread[threadsize]),
        threadfunc(lhs.threadfunc),
        mutex(),
        cond(mutex)
    {};

    ThreadPool& ThreadPool::operator=(const ThreadPool& lhs)
    {
        assert(!started);
        delete [] threadlistptr;
        threadlistptr = new Thread[threadsize];
            
        threadfunc = lhs.threadfunc;
        taskqueue.clear();

        return *this;
    }

    void ThreadPool::start()
    {
        if(!started)
        {
            for(int i=0;i<threadsize;++i)
            {
                threadlistptr[i].setThreadCallBack(threadfunc);
                threadlistptr[i].start();
            }
            started = true;
        }
    }

    void ThreadPool::joinAll()
    {
        assert(started);
        for(int i=0;i<threadsize;++i)
            threadlistptr[i].join();
    }

    void ThreadPool::addInTaskQueue(const ThreadFunc& task)
    {
        if(started)
        {
            MutexLockGuard guard(mutex);
            taskqueue.push_back(task);
        
            cond.notify();
        }
    }

    ThreadFunc ThreadPool::getTaskInTaskQueue()
    {
        MutexLockGuard guard(mutex);
        while(taskqueue.empty())
            cond.wait();
        
        ThreadFunc func(taskqueue.front());
        taskqueue.pop_front();
        return func;
    }
}
