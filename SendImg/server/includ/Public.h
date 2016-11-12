/*************************************************************************
	> File Name: Public.h
	> Author: 
	> Mail: 
	> Created Time: 2016年10月23日 星期日 18时44分15秒
 ************************************************************************/

#ifndef _PUBLIC_H
#define _PUBLIC_H

/*********************C常用库**********************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

/********************文件操作************************/
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<stddef.h>
#include<pwd.h>
#include<sys/types.h>
#include<sys/sendfile.h>
#include<sys/mman.h>

/********************套接字通信模块*********************/
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<sys/time.h>
#include<signal.h>
#include<poll.h>
#include<sys/epoll.h>
#include<errno.h>
#include<signal.h>

/********************线程**************************/
#include<pthread.h>
#include<time.h>
#include<limits.h>
#include<stdarg.h>

/********************常量******************/
static const int LISTEN = 1024;//监听套接字队列等待长度
static const int SERV_PORT = 9998;//服务端接口
static const int SIZE = 64;
static const int DATASIZE = 1224;
static const int PACKETSIZE = 1292;

/*******************系统共有函数************************/
int Readn(int fd,void* ptr,int n);//封装读写系统函数
int Writen(int fd,void* ptr,int n);
int SetNonBlocking(int fd);//设置非阻塞

//异步IO函数
//int judge_epoll(const int& fd,int& epollfd,epoll_event& event);
int ResetEpoll(const int& fd,int& epollfd,epoll_event& event);
int BuildEpoll(const int& fd,int& epollfd,epoll_event& event);
int JudgeEpoll(const int& fd);

struct Packet//传送包1292
{
    char filename[SIZE];
    uint32_t filesize;
    char data[DATASIZE];
};

struct File//套接字传输结构体
{
    struct Packet poet;
    struct File* next;
};

struct Node//链表队列结构体，由服务器端操作，两个指针控制头尾节点
{
    struct File* head;
    struct File* tail;
    pthread_mutex_t Lock;
};

#endif
