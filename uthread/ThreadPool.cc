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
        for(auto iter=threadmap.begin();iter!=threadmap.end();++iter)
            ::pthread_detach(iter->first);
    }

    void ThreadPool::start()
    {
        for(int i=0;i<threadsize;++i)
        {
            Thread thread(threadfunc);
            thread.start();
            threadmap.insert({thread.getThreadId(),thread});
        }
    }

    void ThreadPool::joinAll()
    {
        for(auto iter=threadmap.begin();iter!=threadmap.end();++iter)
        {
            ::pthread_join(iter->first,NULL);
            threadmap.erase(iter);
        }
    }

    void ThreadPool::addInTaskQueue(const ThreadFunc& task)
    {
        MutexLockGuard guard(mutex);
        taskqueue.push_back(task);
        
        cond.notify();
    }

    ThreadFunc&& ThreadPool::getTaskInTaskQueue()
    {
        MutexLockGuard guard(mutex);
        while(taskqueue.empty())
            cond.wait();
        
        ThreadFunc func(taskqueue.front());
        taskqueue.pop_front();
        return std::move(func);
    }
}
