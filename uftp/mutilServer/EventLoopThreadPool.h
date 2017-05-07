/*************************************************************************
	> File Name: EventLoopThreadPool.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月28日 星期二 16时55分50秒
 ************************************************************************/

#ifndef _EVENTLOOPTHREADPOOL_H
#define _EVENTLOOPTHREADPOOL_H

#include"EventLoopThread.h"

namespace unet
{
    namespace thread
    {
        class EventLoopThreadPool final
        {
            typedef std::function<void()> ThreadFunc;
            typedef std::vector<pthread_t> EventLoopThreadList;
            typedef std::vector<unet::net::Epoller*> EpollerList;
            typedef EpollerList::iterator EpollerListIter;
            typedef std::vector<EventLoopThread> LoopThreadList;

            public:
                explicit EventLoopThreadPool(int size = 2);
                EventLoopThreadPool(const EventLoopThreadPool& lhs) = delete;
                EventLoopThreadPool& operator=(const EventLoopThreadPool& lhs) = delete;
                ~EventLoopThreadPool();
           
        //public interface
                void start();
                void joinAll(); 

                void addInChannelMap(unet::net::Channel* channel);

            private:
//                LoopThreadList threads;
                EventLoopThread* threads;
                const int threadsize;
                EventLoopThreadList threadlist;
                EpollerList epollerlist;
                EpollerListIter iter;
        };
    }
}

#endif

