/*************************************************************************
	> File Name: ThreadPool.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月27日 星期一 23时19分53秒
 ************************************************************************/

#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include"Thread.h"
#include<deque>
#include<map>
#include<memory>

namespace unet
{
    namespace thread
    {
        class ThreadPool final
        {
            typedef std::function<void()> ThreadFunc;
            typedef std::map<pthread_t,Thread> ThreadMap;

            public:
                ThreadPool(int size);
                
                ThreadPool(const ThreadPool& lhs) = delete;
                ThreadPool& operator=(const ThreadPool& lhs) = delete;
                
                ~ThreadPool();

//public interface
                void setThreadCallBack(Thread&& cb)
                {
                    thread = cb;
                };

                void setThreadCallBack(const Thread& cb)
                {
                    thread = cb;
                }

                void start();
                void joinAll();

            private:
                const int threadsize;
                ThreadMap threadmap;
                Thread thread;
        };
    }
}






























#endif

