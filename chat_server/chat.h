/**********************************************************************
	> File Name: chat.h
	> Author: UUChen
	> Mail: 17751730329@163.com
	> Created Time: 2016年09月19日 星期一 03时34分29秒
 *********************************************************************/

#ifndef _CHAT_H
#define _CHAT_H

/********C++常用库**********/
#include<string>
#include<vector>
#include<map>
#include<queue>
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
#include<sys/epoll.h>//poll
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
int setnonblocking(int);

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

class PthreadPoll;

class Server
{//暂时先不实现删除好友功能
    friend class ThreadPoll;
    private:
        int listenfd;//监听套接字
        int pretendfd;//伪装套接字
    public:
        Server();
        Server& operator=(const Server& rhs);

        int setlistenfd()
        {
            listenfd = pretendfd;
            return 0;
        }
        
        int setpretendfd()
        {
            pretendfd = listenfd;
            return 0;
        }
//逻辑函数(由对象调用)       
        int if_listenfd_read();//处理链接信息
        int create_connet();//创建连接
        int handle_message(int connetfd);//处理消息
};

class Thread;

std::queue<int> task_list;//工作队列
pthread_mutex_t task = PTHREAD_MUTEX_INITIALIZER;
std::map<std::string,int> name_confd;//线程共享数据
std::vector<Thread*> thread_list;//工作线程队列

class Thread
{
    friend class ThreadPoll;
    friend class Server;
    private:
        pthread_t tid;
        static void* bios(void* arg)
        {
            int waitfd;
            int allfd = 0;
            Server server = (*(Server*)arg);
            server.setlistenfd();
            int epollfd = epoll_create(256);

            while(1)
            { 
                epoll_event event;
                epoll_event eventss[256];

                pthread_mutex_lock(&task);//得到需要处理的文件描述符
                if(task_list.empty())
                {
                    pthread_mutex_unlock(&task);
                    continue;
                }
                else
                {   
                    if(allfd < 256)
                    {
                        waitfd = task_list.front();
                        task_list.pop();
                        ++allfd;
                    }
                        pthread_mutex_unlock(&task);
                }
                event.data.fd = waitfd;//注册新的描述符
                event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                epoll_ctl(epollfd,EPOLL_CTL_ADD,waitfd,&event);
        
                int ret = epoll_wait(epollfd,eventss,256,-1);
                if(ret < 0)
                    continue;
                for(int i=0;i<ret;++i)
                {//epoll模型用来处理一次数据读取
                    int n,fds;
                    fds = eventss[i].data.fd;
                    n = server.handle_message(fds);
                    if(n == 0)//链接断开
                    { 
                        event.data.fd = waitfd;//重置epoll
                        event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                        epoll_ctl(epollfd,EPOLL_CTL_MOD,waitfd,&event);
                    }
                    else
                    {
                        epoll_ctl(epollfd,EPOLL_CTL_DEL,fds,&event);
                        --allfd;
                    }
                }
            }
        }
            
    public:
        Thread() {};
        int Start();
};


class ThreadPoll//线程共享数据只能放在外面，不能作为私有数据
{
    private:
        const static int threadsize = 16;//最大线程数
        const static int thread_user = 512;//每个线程处理的最多客户数
    public:
        ThreadPoll(){};
        ~ThreadPoll();
        int Start();//创建线程对象
};



#endif
