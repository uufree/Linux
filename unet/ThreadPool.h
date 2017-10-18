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
#include"Channel.h"

#include<deque>
#include<memory>

namespace unet
{
    namespace thread
    {
        class ThreadPool final
        {
            typedef std::function<void()> ThreadFunc;

            public:
                explicit ThreadPool(int size = 2);
                explicit ThreadPool(int size,const ThreadFunc& cb);
                ThreadPool(const ThreadPool& lhs) = delete;
                ThreadPool(ThreadPool&& lhs);
                ThreadPool& operator=(const ThreadPool& lhs) = delete;
                ThreadPool& operator=(ThreadPool&& lhs);
                ~ThreadPool();

                void setThreadCallBack(const ThreadFunc& cb)
                {
                    if(!started)
                        threadFunc = cb;
                }

                void setThreadCallBack(ThreadFunc&& cb)
                {
                    if(!started)
                        threadFunc = std::move(cb);
                }

                void start();
                void joinAll();

            private:
                bool started;
                const int threadSize;
                Thread* threadListPtr;
                ThreadFunc threadFunc;
                
                MutexLock mutex;
                Condition cond;
        };
    }
}
#endif

