/*************************************************************************
	> File Name: TaskPool.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月12日 星期三 20时52分01秒
 ************************************************************************/

#ifndef _TASKPOOL_H
#define _TASKPOOL_H

/* 设计理念：以线程为基本对象的不可更改的任务池，为Servre服务
 *
 * 1.基于细粒度的的线程实现
 * 2.不具有可更改性
 * 3.左值引用与右值引用不是万能的，必要的数据依旧需要用指针来操作
 */

#include"Channel.h"
#include"Thread.h"
#include"Mutex.h"
#include"Condition.h"

#include<deque>
#include<memory>
#include<vector>

namespace unet
{
    namespace thread
    {
        class TaskPool final
        {
            typedef std::function<void()> ThreadFunc;
            typedef std::shared_ptr<net::Channel> ChannelPtr;
            typedef std::vector<ChannelPtr> ChannelList;

            public:
                explicit TaskPool(int size = 2);
                explicit TaskPool(int size,const ThreadFunc& cb);
                TaskPool(const TaskPool& lhs) = delete;
                TaskPool(TaskPool&& lhs);
                TaskPool& operator=(const TaskPool& lhs) = delete;
                TaskPool& operator=(TaskPool&& lhs);
                ~TaskPool();

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
                void addInTaskQueue(ChannelList& lhs);

            private:
                void ThreadFunction()
                {
                    ChannelList channels;
                    
                    while(1)
                    {
                        MutexLockGuard guard(mutex);
                        while((channelList.size()) == 0)   
                            cond.wait();
                        guard.~MutexLockGuard();
                        
                        channels.clear();
                        
                        {
                            MutexLockGuard guard(mutex);
                            std::swap(channels,channelList);
                        }
                        
                        for(auto iter=channels.begin();iter!=channels.end();++iter)
                        {
                           (*iter)->handleEvent(); 
                        }
                    }
                }

            private:
                bool started;
                const int threadSize;
                Thread* threadListPtr;
                ThreadFunc threadFunc;
                ChannelList channelList;
                
                MutexLock mutex;
                Condition cond;
        };
    }
}

#endif


