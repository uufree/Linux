/*************************************************************************
	> File Name: EventMap.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月07日 星期五 13时10分57秒
 ************************************************************************/

#include"EventMap.h"
#include<iostream>

namespace unet
{
    namespace net
    {
        EventMap::EventMap()
        {};

        EventMap::EventMap(EventMap&& lhs) :
            eventMap(std::move(lhs.eventMap))
        {};

        EventMap& EventMap::operator=(EventMap&& lhs)
        {
            eventMap = std::move(lhs.eventMap);
            return *this;
        }

        EventMap::~EventMap()
        {};

        void EventMap::swap(EventMap& lhs)
        {
            std::swap(eventMap,lhs.eventMap);
        };

        void EventMap::insert(int fd,int event_,int epollfd)
        {
            struct epoll_event event;
            bzero(&event,sizeof(event));
            event.events = event_;
            event.data.fd = fd;
            
            if(::epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event) < 0)
            {
                unet::handleError(errno);
            }

            {
                thread::MutexLockGuard guard(mutex);
                eventMap.insert({fd,event});
            }
        }

        void EventMap::erase(int fd,int epollfd)
        {
            thread::MutexLockGuard guard(mutex);

            if(::epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,NULL) < 0)
            {
                unet::handleError(errno);
            }
            
            eventMap.erase(fd);
        }
    }
}

