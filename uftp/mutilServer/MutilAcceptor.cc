/*************************************************************************
	> File Name: MutilAcceptor.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月29日 星期三 01时29分59秒
 ************************************************************************/

#include"MutilAcceptor.h"

namespace unet
{
    namespace net
    {
        MutilAcceptor::MutilAcceptor(unet::net::InetAddress* addr_,int size) :
            pool(new unet::thread::EventLoopThreadPool(size)),
            serveraddr(addr_),
            listenfd(socket::socket()),
            listenchannel(new Channel(listenfd,LISTEN)),
            listening(false)
        {
            int fd = socket::socket();
            socket::bind(fd,serveraddr);//设置监听套接字 
            listenfd = fd;
            listenchannel->setFd(fd);
            
            listenchannel->setReadCallBack(std::bind(&MutilAcceptor::handleRead,this));//处理listenfd的可读事件
        }
        
        MutilAcceptor::~MutilAcceptor()
        {
            pool->joinAll();
            ::close(listenfd);
        }

        void MutilAcceptor::listen()
        {
            listening = true;
            socket::listen(listenfd);
            addinserverloop(listenchannel);
        }

        void MutilAcceptor::handleRead()
        { 
            int confd = socket::accept(listenfd);
            assert(confd >= 0);
            socket::setNonBlockAndCloseOnExec(confd);
            
            Channel* channel = nullptr;
            if(newconnectioncallback)
                channel = newconnectioncallback(confd);//获得新的channel
            else
                perror("没有注册newconnectioncallback回调!\n");
            
            pool->addInChannelMap(channel);//将channel添加到epoller中
        }
    }
}

