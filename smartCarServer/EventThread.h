/*************************************************************************
	> File Name: EventThread.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年10月19日 星期四 22时21分33秒
 ************************************************************************/

#ifndef _EVENTTHREAD_H
#define _EVENTTHREAD_H

#include"ChannelMap.h"
#include"Thread.h"
#include"Epoller.h"
#include"EventLoop.h"
#include"EventMap.h"
#include"TaskQueue.h"

namespace unet
{
    namespace net
    {
        typedef std::function<void()> Func;

        class EventThread final
        {
            public:
                explicit EventThread();
                EventThread(const EventThread& lhs) = delete;
                EventThread(EventThread&& lhs);
                EventThread& operator=(const EventThread& lhs) = delete;
                EventThread& operator=(EventThread&& lhs);
                ~EventThread();

                void start();
                void stop();
                void addTask();
                bool isStart() {return started;};
            
            private:
                void InsertChannel(ChannelPtr&& channel);
                void EraseChannel(int index);
                void GetActiveChannels();
                void ThreadStart();

            private:
                ChannelMap channelMap;
                EventMap eventMap;

                Epoller epoller;
                EventLoop eventLoop;
                thread::TaskQueue<Func> taskQueue;        
                thread::Thread thread;
                bool started;
        };
    }
}



#endif
