/*************************************************************************
	> File Name: Typedef.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月19日 星期日 13时18分02秒
 ************************************************************************/

#ifndef _TYPEDEF_H
#define _TYPEDEF_H

#include<functional>
#include<vector>
#include<map>
#include<memory>
#include<sys/epoll.h>
#include<stdio.h>
#include<assert.h>
#include<sys/poll.h>
#include<deque>
#include<iostream>
#include<string.h>


/*
class Channel;
class Buffer;
class TcpConnection;

namespace unet
{
    namespace net
    {
//Acceptor
        typedef std::function<Channel* (int sockfd)> NewConnectionCallBack;
        typedef std::vector<Channel*> ChannelList;

//TcpConnection       
        typedef std::function<void (Buffer* inputbuffer_,Buffer* outputbuffer_)> MessageCallBack;
        typedef std::function<void()> ResetChannelPtr;
        typedef std::function<void(int fd)> ChangeTcpMapIndex;

//Epoller 
        typedef std::vector<struct epoll_event> EventList;
        typedef std::map<int,Channel*> ChannelMap;
    
//TcpServer
        typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
        typedef std::map<int,TcpConnectionPtr> TcpConnectionPtrMap;//保存描述连接的智能指针，索引是连接中保存的fd
    
//EventLoop
        typedef std::function<void(ChannelList*)> GetActiveChannelsCallBack;
        
//Channel        
        typedef std::weak_ptr<TcpConnection> TcpConnectionWptr;
        typedef std::function<void()> EventCallBack; 
        typedef std::function<void(Channel* channel_)> UpdateCallBack;
        typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;     
    }
}

*/

#endif

