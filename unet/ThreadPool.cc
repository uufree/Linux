/*************************************************************************
	> File Name: ThreadPool.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月27日 星期一 23时31分49秒
 ************************************************************************/

#include"ThreadPool.h"

namespace unet
{
    namespace thread
    {
        ThreadPool::ThreadPool(int size) :
            started(false),
            threadSize(size),
            threadListPtr(new Thread[size]),
            mutex(),
            cond(mutex)
        {};
        
        ThreadPool::ThreadPool(int size,const ThreadFunc& cb) :
            started(false),
            threadSize(size),
            threadListPtr(new Thread[size]),
            threadFunc(cb),
            mutex(),
            cond(mutex)
        {};

        ThreadPool::ThreadPool(ThreadPool&& lhs) : 
            started(false),
            threadSize(lhs.threadSize),
            threadListPtr(new Thread[threadSize]),
            threadFunc(std::move(lhs.threadFunc)),
            mutex(),
            cond(mutex)
        {};
        
        ThreadPool& ThreadPool::operator=(ThreadPool&& lhs)
        {
            joinAll();
            delete [] threadListPtr;

            started = false;
            threadListPtr = new Thread[threadSize];
            threadFunc = std::move(lhs.threadFunc);
            return *this;
        }

        ThreadPool::~ThreadPool()
        {
            started = false;
            for(int i=0;i<threadSize;++i)
                ::pthread_detach(threadListPtr[i].getThreadId());

            delete [] threadListPtr;
        }
    
        void ThreadPool::start()
        {
            if(!started)
            {
                for(int i=0;i<threadSize;++i)
                {
                    threadListPtr[i].setThreadCallBack(threadFunc);
                    threadListPtr[i].start();
                }
                started = true;
            }
        }

        void ThreadPool::joinAll()
        {
            assert(started);
            for(int i=0;i<threadSize;++i)
                threadListPtr[i].join();
        }
    }
}
