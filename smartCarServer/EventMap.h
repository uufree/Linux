/*************************************************************************
	> File Name: EventMap.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月07日 星期五 13时04分02秒
 ************************************************************************/

//保存事件的容器，由Server统一调度

#ifndef _EVENTMAP_H
#define _EVENTMAP_H

#include<vector>
#include<sys/epoll.h>
#include<algorithm>
#include<memory>
#include<map>

#include"Mutex.h"

namespace unet
{
    namespace net
    {
        typedef std::vector<struct epoll_event> EventList;

        class EventMap final
        {
            public:
                explicit EventMap();
                EventMap(const EventMap& lhs) = delete;
                EventMap(EventMap&& lhs);
                EventMap& operator=(const EventMap& lhs) = delete;
                EventMap& operator=(EventMap&& lhs);
                ~EventMap();
            
                inline void swap(EventMap& lhs);

                int size() const
                {return eventMap.size();};

                bool empty() const
                {return eventMap.empty();};

                void insert(int fd,int event_,int epollfd);
                void erase(int fd,int epollfd);
                void clear();

            private:
                thread::MutexLock mutex;
                std::map<int,struct epoll_event> eventMap;
        };
    }
}

#endif
