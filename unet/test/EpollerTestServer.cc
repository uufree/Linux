/*************************************************************************
	> File Name: EpollerTestServer.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月19日 星期三 15时02分08秒
 ************************************************************************/

#include<iostream>
#include<sys/epoll.h>
#include<vector>
#include"../Socket.h"
#include"../Buffer.h"

int main(int argc,char** argv)
{
    int epollfd = epoll_create(65536);
    std::cout << "epollfd: " << epollfd << std::endl;

    std::vector<struct epoll_event> events;
    events.reserve(65536);

    unet::net::socket::InetAddress serveraddr(7777);
    unet::net::socket::Socket listenfd(unet::net::socket::LISTEN);
    unet::net::socket::bind(listenfd,serveraddr);
    unet::net::socket::listen(listenfd);
    std::cout << "listenfd: " << listenfd.getFd() << std::endl;
    
    struct epoll_event event;
    bzero(&event,sizeof(event));
    event.events = EPOLLIN;
    event.data.fd = listenfd.getFd();
    epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd.getFd(),&event);
    
    while(1)
    {
        int es = epoll_wait(epollfd,&*events.begin(),65536,-1); 
        std::cout << "es: " << es << std::endl;
        std::cout << "activeEvents: " << events[0].data.fd << std::endl;

        int confd = unet::net::socket::accept(listenfd);
        std::cout << "confd: " << confd << std::endl;
    }

    return 0;
}

