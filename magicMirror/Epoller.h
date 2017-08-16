/*************************************************************************
	> File Name: Epoller.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月09日 星期四 00时00分59秒
 ************************************************************************/

/*设计理念：Epoller仅负责事件分离，将已发生的事件与数据集合相关联
 * 
 * 1.Epoller将分离的Channel依旧保存在Server的可见域内
 * 2.由Server使用并且将处理后的数据集交由Server进一步处理
 * 3.可加入回调函数增加Epoller的灵活性
 * 4.加入的回调函数依旧处理Server的数据集
 */ 


#ifndef _EPOLLER_H
#define _EPOLLER_H

#include<sys/epoll.h>
#include<vector>

#include"ChannelMap.h"
#include"error.h"

namespace unet
{
    namespace net
    {
        static const int timeoutMs = 200;//默认poll阻塞200ms
        
        class Epoller final
        {
            typedef std::vector<ChannelPtr> ChannelList;
            typedef std::vector<struct epoll_event> EventList;

            public:    
                explicit Epoller();
                Epoller(const Epoller&) = delete;
                Epoller& operator=(const Epoller&) = delete;
                Epoller(Epoller&& lhs);
                Epoller& operator=(Epoller&& lhs);
                ~Epoller();

                void epoll(ChannelMap& channelMap);
                
                int getEpollfd()
                {return epollfd;};

            private:
                void getActiveEvents(int activeEvents,ChannelMap& channelMap);

            private:
                int epollfd;
                EventList eventList;
        };    
    }
}


#endif

