/*************************************************************************
	> File Name: TimerQueue.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月28日 星期五 17时53分09秒
 ************************************************************************/

#include"TimerQueue.h"
#include"../unetComponent/error.h"


namespace unet
{
    namespace time
    {
        int createTimefd()
        {
            int timefd = ::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
            if(timefd < 0)
                unet::handleError(errno);
            return timefd;
        }

        struct timespec howMuchTimeFromNow(const Timestamp& lhs)
        {
            int64_t mic = lhs.getTime() - Timestamp().getTime();
            if(mic < 100)
                mic = 100;

            struct timespec ts;
            ts.tv_sec = static_cast<time_t>(mic / time::KMicroseconds);
            ts.tv_nsec = static_cast<long>((mic % time::KMicroseconds)*1000);

            return ts;
        }
        
        TimerQueue::TimerQueue() : 
            timefd(createTimefd()),
            started(false),
            mutex()
        {};

        TimerQueue::TimerQueue(TimerQueue&& lhs) : 
            timefd(createTimefd()),
            started(false),
            mutex(),
            insertChannelCallBack(std::move(lhs.insertChannelCallBack)),
            eraseChannelCallBack(std::move(lhs.eraseChannelCallBack))
        {};

        TimerQueue& TimerQueue::operator=(TimerQueue&& lhs)
        {
            timerMap.clear();
            timerMap = std::move(lhs.timerMap);
            started = false;

            return *this;
        }

        
        TimerQueue::~TimerQueue()
        {
            ::close(timefd);
            timerMap.clear();
        }

        void TimerQueue::addTimer(TimerPtr&& timer_)
        {
            Timestamp now;
            now.addTime(timer_->getRepeatTime());
            
            if(timerMap.size() > 0)
            {//setTimer
                if(now < timerMap.begin()->first)
                {
                    struct itimerspec newSpec;
                    bzero(&newSpec,sizeof(newSpec));
                    struct itimerspec oldSpec;
                    bzero(&oldSpec,sizeof(oldSpec));
                    newSpec.it_value = howMuchTimeFromNow(now);
                
                    if(::timerfd_settime(timefd,0,&newSpec,&oldSpec) < 0)
                        unet::handleError(errno);
                }
            }
            else if(timerMap.size() == 0)
            {
                struct itimerspec newSpec;
                bzero(&newSpec,sizeof(newSpec));
                struct itimerspec oldSpec;
                bzero(&oldSpec,sizeof(oldSpec));
                newSpec.it_value = howMuchTimeFromNow(now);
                
                if(::timerfd_settime(timefd,0,&newSpec,&oldSpec) < 0)
                    unet::handleError(errno);
            }
            else
            {};

            {
                unet::thread::MutexLockGuard guard(mutex);
                timerMap.insert({now,timer_});
            }
        }        

        void TimerQueue::addTimer(Timestamp&& time_,TimerPtr&& ptr)
        {
            if(timerMap.size() > 0)
            {//setTimer
                if(time_ < timerMap.begin()->first)
                {
                    struct itimerspec newSpec;
                    bzero(&newSpec,sizeof(newSpec));
                    struct itimerspec oldSpec;
                    bzero(&oldSpec,sizeof(oldSpec));

                    newSpec.it_value = howMuchTimeFromNow(time_);
                    if(::timerfd_settime(timefd,0,&newSpec,&oldSpec) < 0)
                        unet::handleError(errno);
                }
            }
            else if(timerMap.size() == 0)
            {
                struct itimerspec newSpec;
                bzero(&newSpec,sizeof(newSpec));
                struct itimerspec oldSpec;
                bzero(&oldSpec,sizeof(oldSpec));

                newSpec.it_value = howMuchTimeFromNow(time_);
                if(::timerfd_settime(timefd,0,&newSpec,&oldSpec) < 0)
                    unet::handleError(errno);
            }
            else
            {};


            {
                unet::thread::MutexLockGuard guard(mutex);
                timerMap.insert({time_,ptr});
            }
        }

        void TimerQueue::handleRead()
        {
            ::read(timefd,NULL,0);
            
            TimestampList list;
            Timestamp now;
            
            for(auto iter=timerMap.begin();iter!=timerMap.end();++iter)
            {
                if(iter->first < now)
                {
                    iter->second->run();
                    
                    if(iter->second->isRepeat())
                    {
                        list.push_back(iter->first);
                        Timestamp time = iter->first;
                        time.addTime(iter->second->getRepeatTime());
                        addTimer(std::move(time),std::move(iter->second));
                    }                        
                }
                else
                    break;
            }

            {
                unet::thread::MutexLockGuard guard(mutex);
                for(auto iter=list.cbegin();iter!=list.cend();++iter)
                    timerMap.erase(*iter);
            }
        }

        void TimerQueue::start()
        {
            ChannelPtr channel(new net::Channel(timefd,net::CLOCK));
            channel->setReadCallBack(std::bind(&TimerQueue::handleRead,this));
            
            insertChannelCallBack(std::move(channel));
        }

        void TimerQueue::stop()
        {
            eraseChannelCallBack(timefd);
        }

    }
}

