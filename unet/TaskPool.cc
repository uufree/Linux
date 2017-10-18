/*************************************************************************
	> File Name: TaskPool.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月12日 星期三 20时55分39秒
 ************************************************************************/


#include"TaskPool.h"

namespace unet
{
    namespace thread
    {
        TaskPool::TaskPool(int size) :
            started(false),
            threadSize(size),
            threadListPtr(new Thread[size]),
            mutex(),
            cond(mutex)
        {
            setThreadCallBack(std::bind(&TaskPool::ThreadFunction,this));
        };
        
        TaskPool::TaskPool(int size,const ThreadFunc& cb) :
            started(false),
            threadSize(size),
            threadListPtr(new Thread[size]),
            threadFunc(cb),
            mutex(),
            cond(mutex)
        {};

        TaskPool::TaskPool(TaskPool&& lhs) : 
            started(false),
            threadSize(lhs.threadSize),
            threadListPtr(new Thread[threadSize]),
            threadFunc(std::move(lhs.threadFunc)),
            mutex(),
            cond(mutex)
        {};
        
        TaskPool& TaskPool::operator=(TaskPool&& lhs)
        {
            joinAll();
            delete [] threadListPtr;

            started = false;
            threadListPtr = new Thread[threadSize];
            threadFunc = std::move(lhs.threadFunc);
            return *this;
        }

        TaskPool::~TaskPool()
        {
            started = false;
            for(int i=0;i<threadSize;++i)
                ::pthread_detach(threadListPtr[i].getThreadId());

            delete [] threadListPtr;
            
        }
    
        void TaskPool::start()
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

        void TaskPool::joinAll()
        {
            assert(started);
            for(int i=0;i<threadSize;++i)
                threadListPtr[i].join();
        }

        void TaskPool::addInTaskQueue(ChannelList& tasks)
        {
            MutexLockGuard guard(mutex);   
            std::swap(tasks,channelList);
            cond.notifyAll(); 
        }
    }
}
