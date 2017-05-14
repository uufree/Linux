/*************************************************************************
	> File Name: Epoller.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月09日 星期四 00时00分59秒
 ************************************************************************/

#ifndef _EPOLLER_H
#define _EPOLLER_H

#include"Channel.h"
#include"Mutex.h"

//struct epoll_event;

//用的是Poll,因为epoll老是莫名其妙的报错，检查errno也没用

//用map保存Channel，维护map和channel对应的pollfd

struct pollfd;

namespace unet
{
    namespace net
    { 
        class Epoller final
        {
            static const int timeoutMs = 200;//默认poll阻塞200ms
            static const int KNoneEvent = 0;

//            typedef std::vector<struct epoll_event> EventList;
            typedef std::vector<struct pollfd> EventList;
            typedef std::map<int,Channel*> ChannelMap;
            typedef std::vector<Channel*> ChannelList;

            public:    
                explicit Epoller();
                Epoller(const Epoller&) = delete;
                Epoller operator=(const Epoller&) = delete;
                ~Epoller();
//public interface 
                //将poll中有事件发生的channel放进channellist
                void epoll(ChannelList* channels);
                bool hasChannel(Channel* channel_) const;
                void removeChannel(Channel* channel_);
                void updateChannel(Channel* channel_); 
                void addInChannelMap(Channel* channel_);
                
                int getConSize() const 
                {return channelmap.size();};

                void getInfo() const;
            
            private:

                void update(Channel* channel_);

                //将pollfd与channel做映射
                void getActiveEvents(int activeEvents,ChannelList* channels);

                EventList eventlist;//保存epollfd的数组
//                int epollfd;//内核维护的epollfd表
                ChannelMap channelmap;//保存Channel的Map
                unet::thread::MutexLock mutex;
        };    
    }
}


#endif

