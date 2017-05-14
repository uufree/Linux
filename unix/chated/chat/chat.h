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
char PATH_CLIENT[NAMESIZE] = "/home/will/unix/chat/client/";
char PATH_SERVER[NAMESIZE] = "/home/will/unix/chat/server/";
char BYE[NAMESIZE] = "sorry, register error";

ssize_t readn(int fd,void* ptr,size_t n);
ssize_t writen(int fd,void* ptr,size_t n);

struct message_packet
{
    char sendname[NAMESIZE];
    char message[MAXLINE];
    char receivename[NAMESIZE];
};

struct online_list
{
    char *name[NAMESIZE];
};

class Client
{
    private:
        std::string name;
        std::string password;
        int socketfd;//连接端口
        std::vector<std::string> friend_list;//好友列表
        std::vector<std::string> online_friend_list;//在线好友列表
//        std::vector<string> group_list;//群列表
        struct sockaddr_in serveraddr;
        struct message_packet message;
    public:
        Client(std::string name_,std::string password_);
        ~Client();
        int writestr_cli(std::string inname,std::string posname);
        int if_socketfd_read();
        int add_friend(std::string friendname);
//        int del_friend(std::string friendname);
        int make_message_packet(std::string mess,std::string receive);
        int show_online_friend_list();//在终端显示在线好友列表
        int send_message(std::string message,std::string receive);
        int receive_message();//接受消息
//        int send_file();//发送文件
//        int create_group_chat();//创建群聊
};

class Server
{//暂时先不实现删除好友功能
    private:
        int listenfd;
        std::vector<std::string> all_client_list;//所有客户列表
        std::vector<std::string> online_client_list;//在线客户列表
        std::vector<message_packet*> inline_message;//离线消息队列
        std::map<std::string,int> name_confd;//链接与名字
    public:
        Server();
//功能函数(在方法内使用)      
        int getfd(std::string name);
        std::string getstr(int fd,int line);
        int writestr(std::string openname,std::string writename);

//逻辑函数(由对象调用)       
        int if_listenfd_read();
        int pty(int connetfd,std::string name,std::string password);
        int log(std::string log);//日志记录
        int create_connet();//创建连接
        int add_friend_list(std::string name,std::string friendname);//增加好友列表
        int send_online_friend_list(std::string name,int connetfd);//发送好友列表
        int handle_message(int connetfd);//将消息添加到消息队列
        int send_message(message_packet*);//发送消息
        int closefd(int connetfd);//关闭链接套接字
        int handle_inline_message();//发送离线消息
        int empty_inline_message();//测试消息队列是否为空
};


#endif
