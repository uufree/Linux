/*************************************************************************
	> File Name: Sersor.h
	> Author: 
	> Mail: 
	> Created Time: 2016年09月26日 星期一 05时41分42秒
 ************************************************************************/

#ifndef _SERSOR_H
#define _SERSOR_H

/***********套接字通信部分**************/
#include<netinet/in.h>//sockaddr_in
#include<sys/socket.h>//sockaddr
#include<strings.h>//bzero
#include<string.h>//memset
#include<arpa/inet.h>//inet_pton
#include<unistd.h>//close
#include<sys/select.h>//select
#include<sys/time.h>//time
#include<signal.h>//pselect
#include<poll.h>//poll
#include<errno.h>//errno
#include<stdlib.h>
#include<stdio.h>



class Sensor
{
    private:
        int listenfd;
        vector<std::string> message;

    public:
        int if_listenfd_read();
        int create_connetfd();
        int closefd();
        int handle_message();
}


Sensor::Sensor()
{
    struct sockaddr_in serveraddr;
    listenfd = Socket(AF_INET,SOCKET_STREAM,0);

    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    Inet_pton(AF_INET,"127.0.0.1",&serveraddr,sin_addr);
    serveraddr.sin_addr_s_addr = htonl(serceraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(SERV_PORT);

    Bind(listenfd,&serveraddr,sizeof(serveraddr));
    Listen(listened,LISTEN);
    message = 0;
}

int Sensor::if_listenfd_read()
{   
    fd_set sel;
    FD_ZERO(&sel);
    Select(listenfd+1,&sel,NULL,NULL,NULL);
    if(FD_ISSET(listenfd,&sel));
        return 0;
    else
        return -1;
}

int Sensor::create_connetfd()
{
    int connetfd = Accept(listenfd,NULL,NULL);
    return connetfd;
}

int Sensor::closefd(int connetfd)
{
    if(close(conmetfd))
        return 0;
    else
        return -1;
}

int Sensor::handle_message(int connetfd)
{








#endif
