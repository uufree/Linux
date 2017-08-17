/*************************************************************************
	> File Name: Channel.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月09日 星期四 20时56分13秒
 ************************************************************************/

#ifndef _CHANNEL_H
#define _CHANNEL_H

/* 设计理念：Channel只关注fd上的事件的处理,不负责管理生命周期
 
 * 1.Channel创建时会分类，根据Channel的类型来处理事件
 * 2.Channel持有指向TcpConnection的指针（生命周期同等长）
 * 3.假设一点：Channel创建以后关注的事件不会再改变
 * 4.Channel的相当于事件的外壳，可升级关注更多的细节
 */

#include<functional>
#include<sys/epoll.h>

namespace unet
{
    namespace net
    {
        static const int KNoneEvent = 0;//关注的事件的处理方式
        static const int KReadEvent = EPOLLIN;
        static const int KWriteEvent = EPOLLOUT;
        
        enum ChannelType{LISTEN,CONNECT,CLOCK};
        
        class Channel final
        {
            typedef std::function<void()> ReadCallBack;
            typedef std::function<void(int)> CloseCallBack;

            public:
                explicit Channel(int fd_,ChannelType type_);
                Channel(const Channel& lhs) = delete;
                Channel(Channel&& lhs);
                Channel& operator=(const Channel& lhs)  = delete;
                Channel& operator=(Channel&& lhs);
                ~Channel();
                
                void handleEvent();
                
                int getFd() const
                {return fd;};

                void setEvent()
                {event = KWriteEvent & KReadEvent;};

                int getEvent() const
                {return event;};

                void setRevent(int revent_)
                {revent = revent_;};
        
                int getType() const
                {return type;};
                
                void setReadCallBack(const ReadCallBack& lhs)
                {
                    if(type == LISTEN || type == CLOCK)
                        readCallBack = lhs;
                };

                void setCloseCallBack(const CloseCallBack& lhs)
                {closeCallBack = lhs;};

                bool isNoneEvent() const
                {return event == KNoneEvent;};

                bool isReading() const
                {return event == KReadEvent;};

                bool isWriting() const
                {return event == KWriteEvent;};
                
                void handleClose()
                {closeCallBack(fd);};

            private:
                int fd;
                int event;//关注的事件
                int revent;//正在发生的事件
                bool handleEventing;
                ChannelType type;

                ReadCallBack readCallBack;
                CloseCallBack closeCallBack;
        };
    }
}

#endif

