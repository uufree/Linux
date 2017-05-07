/*************************************************************************
	> File Name: TimerQueue.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月28日 星期五 17时53分09秒
 ************************************************************************/

#include"TimerQueue.h"
#include<sys/timerfd.h>
#include<functional>

namespace unet
{
    namespace time
    {
        int createTimefd()
        {
            int timefd = ::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
            if(timefd < 0)
                perror("timefd create error!\n");
            return timefd;
        }
        
        TimerQueue::TimerQueue() : 
            timefd(createTimefd()),
            timefdchannel(new unet::net::Channel(timefd,unet::net::CLOCK)),
            handlecalbacking(false),
            lock()
        {
            nowtimer = nullptr;
            timefdchannel->setReadCallBack(std::bind(&TimerQueue::handleRead,this));
        };
        
        TimerQueue::~TimerQueue()
        {
            ::close(timefd);
            if(!activetimers.empty())
            {
                for(auto iter=activetimers.begin();iter!=activetimers.end();++iter)
                    iter->second->run();
            }
        }

        void TimerQueue::addTimer(Timer* timer_)
        {
            unet::thread::MutexLockGuard guard(lock);
            activetimers.insert({Timestamp(),timer_});
        }

        void TimerQueue::handleRead()
        {
            ::read(timefd,NULL,0);
            TimerList list;
            
            unet::thread::MutexLockGuard guard(lock);
            Timestamp now;
            for(auto iter=list.begin();iter!=list.end();++iter)
            {
                if(iter->first < now)
                {
                    iter->second->run();
                    if(iter->second->isRepeat())
                    {
                        nowtimer = iter->second;
                        activetimers.insert({now,nowtimer});
                    }                        
                    activetimers.erase(iter);
                }
                else
                    break;
            }
        }

    }
}

