/*************************************************************************
	> File Name: chat.h
	> Author: UUChen
	> Mail: 17751730329@163.com
	> Created Time: 2016年09月19日 星期一 03时34分29秒
 ************************************************************************/

#ifndef _CHAT_H
#define _CHAT_H

/********C++常用库**********/
#include<string>
#include<vector>
#include<map>
#include<utility>
#include<iostream>
#include<algorithm>
#include<iterator>

/***********文件操作************/
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>

/************套接字通信部分************/
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

/**********线程**************/
#include<pthread.h>
#include<time.h>
#include<limits.h>
#include<stdarg.h>

#ifndef INET_ADDRSTRLEN 
#define INET_ADDRSTRLEN 16  //IPV4地址长度
#endif

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46  //IPV6地址长度
#endif

#define LISTEN 1024  //服务端监听套接字
#define MAXLINE 4096  //读取最大行数
#define BUFFSIZE 4096  //缓冲区长度
#define SERV_PORT 9877  //服务端的chat端口

#define MAXSLEEP 128
#define NAMESIZE 128
char PATH_CLIENT[NAMESIZE] = "/home/will/unix/client/";
char PATH_SERVER[NAMESIZE] = "/home/will/unix/server/";
char BYE[NAMESIZE] = "sorry, register error";

ssize_t readn(int fd,void* ptr,size_t n);//read包裹函数
ssize_t writen(int fd,void* ptr,size_t n);//write包裹函数

struct message_packet//消息包
{
    char sendname[NAMESIZE];
    char message[MAXLINE];
    char receivename[NAMESIZE];
};

struct online_list//在线好友列表
{
    char *name[NAMESIZE];
};

class Client
{
    private:
        std::string name;//用户名
        std::string password;//密码
        int socketfd;//连接端口
        struct sockaddr_in serveraddr;//服务器地址信息
    public:
        Client(std::string name_,std::string password_);
        ~Client();
        int if_socketfd_read();//如果有消息发送过来
        int send_message(std::string message,std::string receivename);//发送消息
        int receive_message();//接受消息
};

class Server
{//暂时先不实现删除好友功能
    private:
        int listenfd;//监听套接字
    public:
        Server();
//逻辑函数(由对象调用)       
        int if_listenfd_read();//处理链接信息
        int create_connet();//创建连接
        int handle_message(int connetfd);//处理消息
};


#endif
