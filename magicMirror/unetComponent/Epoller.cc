/*************************************************************************
	> File Name: Epoller.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月09日 星期四 00时01分25秒
 ************************************************************************/

#include"Epoller.h"
#include<iostream>

namespace unet
{
    namespace net
    {
        Epoller::Epoller() :
            epollfd(::epoll_create(65536)),
            eventList()
        {
            if(epollfd < 0)
            {
                unet::handleError(errno);
            }

            eventList.reserve(65536);
        }                     
        
        Epoller::Epoller(Epoller&& lhs) : 
            epollfd(std::move(lhs.epollfd))
        {};
        
        Epoller& Epoller::operator=(Epoller&& lhs)
        {
            eventList = std::move(lhs.eventList);
            return *this;
        }
        
        Epoller::~Epoller()
        {
            ::close(epollfd);
        }

        void Epoller::epoll(ChannelMap& channelMap)
        {
            eventList.clear();
            int activeEvents = ::epoll_wait(epollfd,&*eventList.begin(),65536,-1);

            if(activeEvents > 0)
                getActiveEvents(activeEvents,channelMap);
            else if(activeEvents == 0)
            {

            }
            else
            {
                unet::handleError(errno);
            }
        }

        void Epoller::getActiveEvents(int activeEvents,ChannelMap& channelMap)
        {
            int fd;

            for(int i=0;i<activeEvents;++i)
            {
                if((eventList[i].events & EPOLLIN) || (eventList[i].events & EPOLLOUT))
                {
                    fd = eventList[i].data.fd;
                    ChannelPtr channel = channelMap.findChannel(fd); 
                    channel->setRevent(eventList[i].events);
                
                    if(channel->getType() == CLOCK)
                    {
                        channel->handleEvent();
                    }
                }
            }
        }
    }
}

