/*********************************************************************
	> File Name: server.h
	> Author: 
	> Mail: 
	> Created Time: 2016年10月13日 星期四 20时23分26秒
 ********************************************************************/

#ifndef _SERVER_H
#define _SERVER_H

/********C++常用库**********/
#include<string>
#include<vector>
#include<map>
#include<deque>
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
#include<netinet/in.h>
#include<sys/socket.h>
#include<strings.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/select.h>
#include<sys/time.h>
#include<signal.h>
#include<sys/epoll.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>

/**********线程**************/
#include<pthread.h>
#include<time.h>
#include<limits.h>
#include<stdarg.h>

//使用static,const有效检验编译期错误
static const int INET_ADDRSTRLEN = 16;//ipv4地址长度
static const int BUFFSIZE = 8192;//缓冲区长度
static const int INET6_ADDRSTRLEN = 46;//ipv6地址长度
static const int LISTEN = 1024;//监听队列长度
static const int SERV_PORT = 8080;//监听套接字端口
static const int MAXCONNECT = 1024;//最大连接数

ssize_t readn(int fd,void* ptr,size_t n);//read包裹函数
ssize_t writen(int fd,void* ptr,size_t n);//write包裹函数
int setnonblocking(int);//设置非阻塞fd

struct httpwait//http事件原型
{
    char requesttype[4];//请求状态
    int urltype;//url静态(0)还是动态(1)
    std::string httpurl;//http url
    char httpmessage[8];//Http版本
    std::vector<std::string> dynamicstr;//动态处理参数
};

class httpwaitlock//用类封装事件队列
{
    private:
        pthread_rwlock_t httplock;
        std::deque<struct httpwait> static_httpwait;
        std::deque<struct httpwait> dynamic_httpwait;
        httpwaitlock(const httpwait&);//不予定义
        httpwaitlock& operator=(const httpwaitlock&);
    public:
        httpwaitlock(){};
        ~httpwaitlock(){};
        int judge_stadeque() const;//判断队列是否为空
        int judge_dyndeque() const;
        int putstalock(const struct httpwait& stahttpwait);//存
        int putdynlock(const struct httpwait& dynhttpwait);
        httpwait getstalock();//取
        httpwait getdynlock();
};

class fdlock//用类封装连接套接字
{
    private:
        pthread_rwlock_t lock;
        std::deque<int> confds;
        fdlock(const fdlock&);//不予定义
        fdlock& operator=(const fdlock&);
    public:
        fdlock(){};
        ~fdlock(){};
        int judgefd() const;
        int putfd(int fd);//存
        int getfd(int fd);//取
};

class Server
{   
    friend class fdlock;
    friend class httpwaitlock;
    private:        
        mutable int listenfd;
        Server(const Server&);
        Server& operator=(const Server&);
    public:
        Server(){};
        int Communication() const;//构建网络通信模块
        int judge_listenfd_read() const;//判断监听套接字是否可读
        struct httpwait analyse() const;//分析http头部,插入处理队列
        int handle_static_url(const struct httpwait& node) const;//处理静态
        int handle_dynamic_url(const struct httpwait& node)const;//处理动态
        int handle_error() const;//处理错误
};

class Thread//线程及线程池原型
{
    friend class fdlock;
    friend class httpwaitlock;
    friend class server;
    private:
        pthread_t tid;
        static void* analyse(void* arg)//这条线程处理解析
        Thread(const Thread&);
        Thread& operator=(const Thraed&);
    public:
        Thread(){};
        int Start();//启动线程
        ~Thread();
};

class ThreadPoll
{
    private:
        const static int threadsize;
        const static int thread_usr;
        ThraedPoll(const ThreadPoll&);
        ThraedPoll& operator=(const ThreadPoll&);
    public:
        ThreadPoll();
        ~ThreadPOll();
        int Start();//启动线程池
};
        
#endif
