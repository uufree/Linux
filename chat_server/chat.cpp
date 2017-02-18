/*********************************************************************
	> File Name: chat.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月20日 星期二 03时08分26秒
 ********************************************************************/



#include"chat.h"
using namespace std;
/***************socket包裹函数*****************/

ssize_t readn(int fd,void* ptr,size_t n)//read包裹函数
{
    size_t nleft;
    ssize_t nread;
    char* cptr;

    cptr = (char*)ptr;
    nleft = n;
    while(nleft > 0)
    {
        if((nread=read(fd,cptr,nleft)) < 0)
        {
            if(errno == EINTR)
                nread = 0; //call again
            else
            {
                cerr << "readn error" << endl;
                return -1; //error
            }
        }
        else if(nread == 0)
            break;  //EOF
        nleft -= nread;
        cptr += nread;
    }

    return (n-nleft);
}

ssize_t writen(int fd,void* vptr,size_t n)//write包裹函数
{
    size_t nleft;
    ssize_t nwritten;
    char* ptr;

    ptr = (char*)vptr;
    nleft = n;
    while(nleft>0)  
    {
        if((nwritten=write(fd,ptr,nleft)) <= 0)
        {
            if(nwritten<0 && errno!=EINTR)
                nwritten = 0;    //call again
            else
            {
                cerr << "writen error" << endl;
                return -1;  //error
            }
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

int setnonblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_GETFL,new_option);
    return 0;
}

/*********************client类方法*************************/
Client::Client(string name_,string password_):name(name_),password(password_)
{
    socketfd = socket(AF_INET,SOCK_STREAM,0);//获得链套接字
    bzero(&serveraddr,sizeof(serveraddr));//初始化服务器地址
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,"127.0.0.1",&serveraddr.sin_addr);//在本机完成测试，使用本机环回地址
   

    char buf_[NAMESIZE];
    if(connect(socketfd,(sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)//链接
        cerr << "connect error" << endl;

    strcpy(buf_,name.c_str());//把客户信息发送到服务端
    writen(socketfd,buf_,NAMESIZE);
    strcpy(buf_,password.c_str());
    writen(socketfd,buf_,NAMESIZE);
    readn(socketfd,buf_,NAMESIZE);//接受登录成功或失败的信息
    cout << buf_ << endl;
}

Client::~Client()
{
    close(socketfd);//关闭链接套接字
}

int Client::send_message(string receivename,string message)
{
    struct message_packet* mess = new message_packet;
    strcpy(mess->sendname,name.c_str());//处理消息包
    strcpy(mess->message,message.c_str());
    strcpy(mess->receivename,receivename.c_str());
    if(writen(socketfd,mess,sizeof(message_packet)) > 0)//发送消息
    {
        delete mess;
        return 0;
    }
    else
    {
        delete mess;
        return -1;
    }
}

int Client::receive_message()//接受消息
{
    struct message_packet packet;
    readn(socketfd,&packet,sizeof(message_packet));
    cout << packet.receivename << " : " << packet.message << endl;
    return 0;
}

int Client::if_socketfd_read()//检查链接套接字是否可读
{
    fd_set sel;
    FD_ZERO(&sel);
    FD_SET(socketfd,&sel);
    select(socketfd+1,&sel,NULL,NULL,NULL);
    if(FD_ISSET(socketfd,&sel))
        return 0;
    else
        return -1;
}

/**********************server类方法***********************/

Server::Server()
{//实例化对象后，监听套接字一直存在
    struct sockaddr_in serveraddr;//服务器地址
    listenfd = socket(AF_INET,SOCK_STREAM,0);//监听套接字
    
    bzero(&serveraddr,sizeof(serveraddr));//初始化服务器地址
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(SERV_PORT); 

    bind(listenfd,(sockaddr*)&serveraddr,sizeof(serveraddr));//关联
    listen(listenfd,LISTEN);//监听套接字
}

Server& Server::operator=(const Server& rhs)
{
    this->listenfd = rhs.listenfd;
    return *this;
}

int Server::create_connet()//创建连接套接字
{
    char buf[NAMESIZE] = "login success";
    char name[NAMESIZE];
    char password[NAMESIZE];
    int connetfd = accept(listenfd,NULL,NULL);
    if(connetfd < 0)
        cerr << "connetfd create error" << endl;
    setnonblocking(connetfd);

    readn(connetfd,name,NAMESIZE);//服务端io处理
    cout << name  << " "<< "conneted!" << endl;
    readn(connetfd,password,NAMESIZE);
    writen(connetfd,buf,NAMESIZE);

    string name_ = name;

    pair<string,int> pir = pair<string,int>(name_,connetfd);
    name_confd.insert(pir);//将服务器信息与端口号关联起来
    
    pthread_mutex_lock(&task);
    task_list.push(connetfd);
    pthread_mutex_unlock(&task);
    return connetfd;
}

int Server::if_listenfd_read()
{//判断监听套接字是否可读，根据是否可读，创建链接套接字
    fd_set sel;
    FD_ZERO(&sel);
    FD_SET(listenfd,&sel);
    select(listenfd+1,&sel,NULL,NULL,NULL);
    if(FD_ISSET(listenfd,&sel))
        return 1;
    else
        return -1;
}

int Server::handle_message(int connetfd)
{       
    int confd;
    string name;

    struct message_packet* message_ = new message_packet;
    if(readn(connetfd,message_,sizeof(message_packet)) > 0)
    {//从套接字中读取信息
        name = message_->receivename;
        map<string,int>::iterator iter;//根据名字查找套接字
        iter = name_confd.find(name);
        if(iter != name_confd.end())
        {
            confd = iter->second;
            writen(confd,message_,sizeof(message_packet));//写入
        }
    }
    else
        return -1;//链接断开
    delete message_;//删除
    return 0;
}



/*******************Thread函数************************/
int Thread::Start()
{
    int err;
    Server server;
    pthread_attr_t attr;
    err = pthread_attr_init(&attr);
    if(err != 0)
    {
        cerr << "pthread attr create error" << endl;
        exit(0);
    }
    err = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    if(err != 0)
    {
        cerr << "attr create error" << endl;
        exit(0);
    }
    err = pthread_create(&tid,&attr,bios,&server);
    if(err != 0)
    {
        cerr << "pthread create error" << endl;
        exit(0);
    }
    return 0;
}


/******************ThreadPoll函数******************w****/
int ThreadPoll::Start()
{
    for(int i=0;i<threadsize;i++)
    {
        Thread* thr = new Thread;
        thr->Start();
        thread_list.push_back(thr);
    }
    return 0;
}

ThreadPoll::~ThreadPoll()
{
    for(int i=0;i<threadsize;i++)
        delete thread_list[i];
}

            

    













































