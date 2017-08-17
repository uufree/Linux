/*************************************************************************
	> File Name: TimerQueue.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月10日 星期五 21时30分32秒
 ************************************************************************/

#ifndef _TIMERQUEUE_H
#define _TIMERQUEUE_H

#include"Timer.h"
#include"Timestamp.h"
#include"../unetComponent/Channel.h"
#include"../unetComponent/Mutex.h"

#include<map>
#include<sys/timerfd.h>
#include<functional>
#include<memory>
#include<vector>
#include<string.h>

namespace unet
{
    namespace time
    {
        typedef std::shared_ptr<Timer> TimerPtr;
        
        class TimerQueue final
        {
            typedef std::map<Timestamp,TimerPtr> TimerMap;
            typedef std::unique_ptr<unet::net::Channel> ChannelPtr;
            typedef std::function<void(ChannelPtr&&)> InsertChannelCallBack;
            typedef std::function<void(int)> EraseChannelCallBack;
            typedef std::vector<Timestamp> TimestampList;

            public:
                TimerQueue();
                TimerQueue(const TimerQueue& lhs) = delete;
                TimerQueue& operator=(const TimerQueue& lhs) = delete;
                TimerQueue(TimerQueue&& lhs);
                TimerQueue& operator=(TimerQueue&& lhs);
                ~TimerQueue();

                void addTimer(TimerPtr&& timer_);
                void addTimer(Timestamp&& time_,TimerPtr&& ptr);
                void start();
                void stop();

                void setInsertChannelCallBack(const InsertChannelCallBack& cb)
                {insertChannelCallBack = cb;};

                void setEraseChannelCallBack(const EraseChannelCallBack& cb)
                {eraseChannelCallBack = cb;};
                

            private:
                void handleRead();
            
            private:
                const int timefd;
                TimerMap timerMap;
                bool started;
                unet::thread::MutexLock mutex;
                InsertChannelCallBack insertChannelCallBack;
                EraseChannelCallBack eraseChannelCallBack;
        };
        
        struct timespec howMuchTimeFromNow(const Timestamp& lhs);
    }
}

#endif

