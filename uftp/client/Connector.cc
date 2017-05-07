/*************************************************************************
	> File Name: Connector.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月21日 星期二 20时36分20秒
 ************************************************************************/

#include"Connector.h"
#include<sys/poll.h>
#include<iostream>

namespace unet
{
    namespace net
    {
        Connector::Connector() : 
            epoller(new Epoller),
            loop(new AsyncEventLoop)
        {
            loop->setGetActiveChannelsCallBack(std::bind(&Connector::getActiveChannels,this,std::placeholders::_1));
        };

        void Connector::getActiveChannels(ChannelList* channels)
        {
            epoller->epoll(channels);
        }

        void Connector::connection(InetAddress* addr_)
        {
/*            
            while(!createConnection())   
            {
                confd.setFd(socket::socket());
                socket::setNonBlockAndCloseOnExec(confd.getFd());
            }
*/
            int confd = socket::socket();
            socket::connect(confd,addr_);            
            socket::setNonBlockAndCloseOnExec(confd);
            std::cout << "confd: " << confd << std::endl;
            
            Channel* connectchannel = connectioncallback(confd);
            epoller->addInChannelMap(connectchannel);
        }

        void Connector::start()
        {
            loop->loop();
        }
/*
        int Connector::createConnection()
        {
            int n = socket::connect(confd.getFd(),serveraddr);
            if(n < 0)
            {
                if(errno != EINPROGRESS)
                {
                    ::close(confd.getFd());
                    return -1;
                }
            }
            else
                return confd.getFd();
            
            struct pollfd pfd;
            pfd.fd = confd.getFd();
            pfd.events = POLLOUT;

            int ret = ::poll(&pfd,1,50);
            if(ret == 1)
            {
                if((pfd.revents & POLLOUT) && pfd.revents & !POLLIN)
                {
                    socklen_t sock;
                    if(getsockopt(confd.getFd(),SOL_SOCKET,SO_ERROR,NULL,&sock) < 0)
                    {
                        ::close(confd.getFd());
                        return -1;
                    }
                    else
                        return confd.getFd();
                }

                else if(pfd.revents & POLLIN)
                {
                    ::close(confd.getFd());
                    return -1;
                }
                else
                {
                    ::close(confd.getFd());
                    return -1;
                }
            }
            else
                return -1;

        }
*/    
    } 
}            








































