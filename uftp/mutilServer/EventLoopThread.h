/*************************************************************************
	> File Name: EventLoopThread.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月28日 星期二 17时07分37秒
 ************************************************************************/

#ifndef _EVENTLOOPTHREAD_H
#define _EVENTLOOPTHREAD_H

#include"../Thread.h"
#include"Current.h"
//将current聚合类和线程类封装在一起，形成一条单独的线程
namespace unet
{
    namespace thread
    {
        class EventLoopThread final
        {
            public:
                explicit EventLoopThread();
                EventLoopThread(const EventLoopThread& lhs);
                EventLoopThread(EventLoopThread&& lhs);
                EventLoopThread& operator=(const EventLoopThread& lhs);
                EventLoopThread& operator=(EventLoopThread&& lhs);
                ~EventLoopThread(){};
                
                void loop(Current* current_);
                void setThreadCallBack();
                
                pthread_t getThreadId()
                {return thread->getThreadId();};

                void start()
                {thread->start();}

                unet::net::Epoller* getEpoller()
                {return current->getEpoller();};

                int getConSize()
                {return current->getConSize();};

            private:
                std::unique_ptr<Current> current;
                std::unique_ptr<Thread> thread;
        };
    }
}














#endif

