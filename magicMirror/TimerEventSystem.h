/*************************************************************************
	> File Name: TimerEventSystem.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年08月16日 星期三 21时06分45秒
 ************************************************************************/

#ifndef _TIMEREVENTSYSTEM_H
#define _TIMEREVENTSYSTEM_H

#include"TimerQueue.h"
#include"ChannelMap.h"
#include"Thread.h"
#include"Epoller.h"
#include"EventLoop.h"
#include"EventMap.h"

namespace unet
{
    namespace net
    {
        class TimerEventSystem final
        {
            public:
                explicit TimerEventSystem();
                TimerEventSystem(const TimerEventSystem& lhs) = delete;
                TimerEventSystem(TimerEventSystem&& lhs);
                TimerEventSystem& operator=(const TimerEventSystem& lhs) = delete;
                TimerEventSystem& operator=(TimerEventSystem&& lhs);
                ~TimerEventSystem();
                
                void start();
                void stop();
                void addTimer(time::TimerPtr&& timer);
                
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
                time::TimerQueue timerQueue;
                thread::Thread thread;
        };
    }
}

#endif
