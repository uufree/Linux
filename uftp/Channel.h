/*************************************************************************
	> File Name: Channel.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月09日 星期四 20时56分13秒
 ************************************************************************/

#ifndef _CHANNEL_H
#define _CHANNEL_H

//核心思想在于Channel不负责fd的生命期，只负责其上事件的处理

//并且Channel拥有TcpConnection对象的weak_ptr，用弱指向判断TcpConnection对象是否还存在

//确信讨论一点，当一个连接建立起来之后，关注的事件应该不会再变了

//confd关注可写和可读事件，listenfd关注可读事件,timefd关注可读事件

//由于listenchannel与普通的channel不同（没有TcpConnection）,用一个bool类型进行判断

#include"TcpConnection.h"

class Channel;

namespace unet
{
    namespace net
    {
        
        enum ChannelType{LISTEN,CONNECT,CLOCK};
        
        class Channel final
        {
            typedef std::weak_ptr<TcpConnection> TcpConnectionWptr;
            typedef std::function<void()> EventCallBack; 
            typedef std::function<void(Channel*)> UpdateCallBack;
            typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;     

            public:
                explicit Channel(int fd_,ChannelType type_);
                ~Channel();
                Channel(const Channel& lhs) = delete;
                Channel& operator=(const Channel& lhs)  = delete;
                
//public interface
                void handleEvent();//处理事件的主体函数
                void handleClose();//处理关闭事件

                void setReadCallBack(const EventCallBack& cb)
                {readcallback = cb;};//在没有TcpConnection的情况下由Listenfd和Timefd注册

                void setUpdateCallBack(const UpdateCallBack& cb)
                {updatecallback = cb;};//Epoller注册的

                int getFd() const 
                {return fd;};//得到fd

                //设置关注的事件，默认关注读写事件，看情况关闭写事件
                void setEvent() {event = KWriteEvent && KReadEvent;};
                
                //得到关注的事件
                int getEvent() const 
                {return event;};

                //设置正在发生的事件
                void setRevent(int revent_) 
                {revent = revent_;};

                //用于判断现在关注什么事件
                bool isNoneEvent() const {return event== KNoneEvent;};
                bool isReading() const {return event == KReadEvent;};
                bool isWriting() const {return event == KWriteEvent;};

                //不关注之后立刻在epoller中更新
                //不关注读事件
                void disableReading() 
                {
                    event|=~KReadEvent;
                    updatecallback(this);//更新操作是关键
                };

                //不关注写事件
                void disableWriting() 
                {
                    event|=~KWriteEvent;
                    updatecallback(this);
                };

                //不关注所有的事件
                void disableAll()
                {
                    event = KNoneEvent;
                    updatecallback(this);
                }
                
                void handleDrived()
                {//如果提升成功，立刻处理由TcpServer注册的事件
                    TcpConnectionPtr ptr = tcpconnectionwptr.lock();
                    if(ptr)
                        ptr->handleDrived();
                }
                
                int asyncRead()
                {
                    int n = 0;
                    TcpConnectionPtr ptr = tcpconnectionwptr.lock();
                    if(ptr)
                        n = ptr->asyncRead();
                    return n;
                }
                
                int getType()
                {return type;};

                //用std::move的特性，将shared_ptr<TcpConnection>交由TcpServer保管
                TcpConnectionPtr&& getTcpConnectionPtr()
                {return std::move(tcpconnectionptr);};
                
                //设置Channel关注的fd
                void setFd(int fd_)
                {fd = fd_;};
                
            private:
                int fd;
                int event;//关注的事件
                int revent;//正在发生的事件
                bool handleeventing;
                TcpConnectionPtr tcpconnectionptr;
                TcpConnectionWptr tcpconnectionwptr;

                static const int KNoneEvent;
                static const int KReadEvent;
                static const int KWriteEvent;
                EventCallBack readcallback;//listenfd与timefd的回调
                UpdateCallBack updatecallback;
                ChannelType type;
        };
    }
}

#endif

