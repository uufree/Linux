/**********************************************************************
	> File Name: debug.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月29日 星期四 01时41分44秒
 *********************************************************************/

/********C++常用库**********/
#include<string>
#include<vector>
#include<map>
#include<utility>
#include<iostream>
#include<iterator>
#include<algorithm>
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
#define NAMESIZE 50
char PATH_CLIENT[NAMESIZE] = "/home/will/unix/chat/client/";
char PATH_SERVER[NAMESIZE] = "/home/will/unix/chat/server/";
char BYE[NAMESIZE] = "sorry, register error";

ssize_t readn(int fd,void* ptr,size_t n);
ssize_t writen(int fd,void* ptr,size_t n);
void* handle(void* arg);

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

using namespace std;

/*class Client
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
};*/

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
        int empty_inline_message();
        int pty(int connetfd,std::string name,std::string password);
        int log(std::string log);//日志记录
        int create_connet();//创建连接
        int add_friend_list(std::string name,std::string friendname);//增加好友列表
        int send_online_friend_list(std::string name,int connetfd);//发送好友列表
        int handle_message(int connetfd);//将消息添加到消息队列
        int send_message(message_packet*);//发送消息
        int closefd(int connetfd);//关闭链接套接字
        int handle_inline_message();//发送离线消息
};


/***************socket包裹函数*****************/

ssize_t readn(int fd,void* ptr,size_t n)
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
                return -1; //error
        }
        else if(nread == 0)
            break;  //EOF
        nleft -= nread;
        cptr += nread;
    }

    return (n-nleft);
}

ssize_t writen(int fd,void* vptr,size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    char* ptr;

    ptr = (char*)vptr;
    nleft = n;
    while(nleft>0)  
    {
        if((nwritten=writen(fd,ptr,nleft)) <= 0)
        {
            if(nwritten<0 && errno!=EINTR)
                nwritten = 0;    //call again
            else
                return -1;  //error
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}


/*********************client类方法*************************/
/*Client::Client(string name_,string password_):name(name_),password(password_)
{
    socketfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,"127.0.0.1",&serveraddr.sin_addr);
    
    char buf_[NAMESIZE];
    connect(socketfd,(sockaddr*)&serveraddr,sizeof(serveraddr));
    strcpy(buf_,name.c_str());
    writen(socketfd,buf_,NAMESIZE);
    strcpy(buf_,name.c_str());
    writen(socketfd,buf_,NAMESIZE);
    
    int fd,n;//初始化好友列表
    char buf[MAXLINE];
    FILE* fp;
    string posname;
    char name__[NAMESIZE];
    strcpy(name__,name.c_str());
    strcat(PATH_CLIENT,name__);
    fd = open(PATH_CLIENT,O_CREAT|O_APPEND|O_RDWR);
    fp = fdopen(fd,"r");
    for(;;)
    {
        fgets(buf,256,fp);
        if(buf != NULL)
        n = strlen(buf);
        if(buf[n-1] == '\n')
            buf[n-1] = '\0';
        posname = buf;
        friend_list.push_back(posname);
    }
    
    struct online_list list;
    readn(socketfd,&list,sizeof(list));//初始化在线好友列表
    string plname;
    for(int i=0;i<25;i++)
    {
        if(list.name[i] != NULL)
        {
            plname = list.name[i];
            online_friend_list.push_back(plname);
        }
    }
}

Client::~Client()
{
    close(socketfd);
}

int Client::add_friend(string friendname)
{
    if(writestr_cli(name,friendname) < 0)
        return -1;
    return 0;
}

int Client::show_online_friend_list()
{
    vector<string>::iterator iter;
    for(iter=online_friend_list.begin();iter!=online_friend_list.end();iter++)
        cout << *iter << endl;
    return 0;
}

int Client::make_message_packet(string mess,string receive)
{
    strcat(message.sendname,name.c_str());
    strcat(message.message,mess.c_str());
    strcat(message.receivename,receive.c_str());
    return 0;

}

int Client::send_message(string message,string receive)
{
    make_message_packet(message,receive);
    if(writen(socketfd,&message,sizeof(message)) > 0)
        return 0;
    else 
        return -1;
}

int Client::receive_message()
{
    struct message_packet packet;
    readn(socketfd,&packet,MAXLINE);
    cout << packet.receivename << " : " << packet.message << endl;
    strcat(packet.receivename," : ");
    strcat(packet.receivename,packet.message);
    string list = packet.receivename;
    writestr_cli("chatnote",list);
    return 0;
}

int Client::writestr_cli(string inname,string posname)
{
    int fd,n;
    char name_[NAMESIZE];
    strcpy(name_,inname.c_str());
    strcat(PATH_CLIENT,name_);
    fd = open(PATH_CLIENT,O_RDWR|O_CREAT|O_APPEND);
    char name__[NAMESIZE];
    strcpy(name__,posname.c_str());
    strcat(name__,"\n");
    n = strlen(name__);
    if(write(fd,name__,n) != n)
    {
        cerr << "writestr_cli error" << endl;
        return -1;
    }
    return 0;
}

int Client::if_socketfd_read()
{
    fd_set sel;
    FD_ZERO(&sel);
    FD_SET(socketfd,&sel);
    select(socketfd+1,&sel,NULL,NULL,NULL);
    if(FD_ISSET(socketfd,&sel))
        return 0;
    else
        return -1;
}*/

/**********************server类方法***********************/
Server::Server()
{//实例化对象后，监听套接字一直存在
    struct sockaddr_in serveraddr;
    listenfd = socket(AF_INET,SOCK_STREAM,0);

    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&serveraddr.sin_addr);
    serveraddr.sin_addr.s_addr = htonl(serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(SERV_PORT);
    
    bind(listenfd,(sockaddr*)&serveraddr,sizeof(serveraddr));
    listen(listenfd,LISTEN);
    
//    online_client_list = 0;
//    online_list = 0;
//    inline_list = 0;
//    name_confd = 0;
}

int Server::create_connet()//创建连接套接字
{
    int connetfd = accept(listenfd,NULL,NULL);
    return connetfd;
}

int Server::if_listenfd_read()
{//判断监听套接字是否可读，根据是否可读，创建链接套接字
    fd_set sel;
    FD_ZERO(&sel);
    FD_SET(listenfd,&sel);
    select(listenfd+1,&sel,NULL,NULL,NULL);
    if(FD_ISSET(listenfd,&sel))
        return 0;
    else
        return -1;
}

int Server::handle_message(int connetfd)
{//处理消息，在线消息直接发送。离线消息在服务器开机情况下保存   
    int n;
    string name,password;
    char cliname[NAMESIZE];
    if(readn(connetfd,cliname,NAMESIZE) > 0)//从对方读取姓名与密码
        name = *cliname;//是否有*
    if(readn(connetfd,cliname,NAMESIZE) > 0)
        password = *cliname;//是否有*

    if(pty(connetfd,name,password) == 0)//登录程序根据信息进行判断
    {//登录成功
        online_client_list.push_back(name);//增加在线好友列表
        all_client_list.push_back(name);//所有登录人员登记
        vector<string>::iterator iter;
        for( ; ; )
        {
            struct message_packet* mp = new message_packet;
            read:
            if((n=readn(connetfd,&mp,MAXLINE)) > 0)//TCP发送struct
            {
                string rm = mp->receivename;
                if(find(online_client_list.begin(),online_client_list.end(),rm) != online_client_list.end())//在在线列表查找是否在线，在线
                {
                    send_message(mp);//发送消息
                    delete mp;
                }
                else//不在线，加入离线消息列表
                    inline_message.push_back(mp);
            }
            else if(n == 0)//客户发送消息结束
            {
                closefd(connetfd);//关闭链接套接字
                return 0;
            }
            else if(n<0)//出现异常
            {
                if(errno == EINTR)//如果是系统中断异常
                    goto read;//跳转，重新读取消息
                else
                {
                    log("read message error");//日志记录
                    closefd(connetfd);//关闭连接套接字
                    return -1;//读错和登陆失败返回-1
                }
            }
        }
    }
    else//登陆失败
    {
        writen(connetfd,BYE,strlen(BYE));
        closefd(connetfd);
        return -1;
    }
}

int Server::pty(int connetfd,string name,string password)//登录程序
{//涉及文件操作部分，需要重写常用函数，放入私有部分
    if(find(all_client_list.begin(),all_client_list.end(),name) != all_client_list.end())//查找是否有登录记录，有
    {
        string getname,getpassword;
        int fd;
        if((fd=getfd(name)) < 0)
        {
            log("getfd error");
            return -1;
        }
        getname = getstr(fd,1);
        getpassword = getstr(fd,2); 
        close(fd);
        if(getname == name && getpassword == password)
        {
            name_confd.insert(pair<string,int>(name,connetfd));
            return 0;
        }
        else
            return -1;
    }
    else//没有登录记录
    {
        if(writestr(name,name) < 0)
        {
            log("writestr error");
            return -1;
        }
        if(writestr(name,password) < 0)
        {
            log("writestr error");
            return -1;
        }
        name_confd.insert(pair<string,int>(name,connetfd));
        all_client_list.push_back(name);
        online_client_list.push_back(name);
        return 0;
    }
}

int Server::log(string log)
{
    char path[NAMESIZE] = "/home/will/unix/chat/server/log.log";
    int fd = open(path,O_APPEND|O_RDWR|O_CREAT);
    char message[NAMESIZE];
    strcpy(message,log.c_str());
    strcat(message,"\n");
    int n = strlen(message);
    write(fd,message,n);
    close(fd);
    return 0;
}
        
int Server::add_friend_list(string name,string friendname)
{
    if(writestr(name,friendname) < 0)
    {
        log("writestr error");
        return -1;
    }
    return 0;
}

int Server::send_online_friend_list(string name,int connetfd)
{
    int fd;
    string geet;
    char buf[256];
    vector<string> allfriend;
    vector<string> sendlist;
    fd = getfd(name);
    FILE* fp = fdopen(fd,"r");
    fgets(buf,256,fp); 
    fgets(buf,256,fp);
    while(fgets(buf,256,fp) != NULL)
    {
        string name = buf;
        allfriend.push_back(name);
    }
    vector<string>::iterator iter_all;  
    vector<string>::iterator iter_online;
    for(iter_all=allfriend.begin();iter_all!=allfriend.end();iter_all++)
    {
        for(iter_online=online_client_list.begin();iter_online!=online_client_list.end();iter_online++)
        {
            if(*iter_all == *iter_online)
                sendlist.push_back(*iter_all);
        }
    }

    struct online_list listop;
    int size = sendlist.size();
    for(int i=0;i<size;i++)
    {
        char hj[NAMESIZE];
        strcpy(hj,sendlist[i].c_str());
        listop.name[i] = hj;
    }
    if(writen(name_confd[name],&listop,sizeof(listop)) > 0)
        return 0;
    else 
        return -1;
}

int Server::empty_inline_message()//离线消息列表是否为空
{
    if(inline_message.empty())
        return 0;
    else 
        return -1;
}

int Server::send_message(message_packet* mp)
{   
    
    int connetfd = name_confd[mp->receivename];
    if(connetfd < 0)
    {
        log("friend don't online");
        exit(0);
    }
    add_friend_list(mp->sendname,mp->receivename);
    if(writen(connetfd,mp,sizeof(message_packet)) < 0)
    {
        log("send_message error");
        return -1;
    }
    return 0;
}

int Server::handle_inline_message()//处理离线消息
{
    vector<message_packet*>::iterator iter;
    for(iter=inline_message.begin();iter!=inline_message.begin();iter++)
    {
        string klname = (**iter).receivename;
        if(name_confd.find(klname) != name_confd.end())
        {
            send_message(*iter);
            delete *iter;
            return 0;
        }
        else
            continue;
    }
    return 0;
}

int Server::closefd(int connetfd)//关闭链接套接字
{
    string name;
    map<string,int>::iterator iter;
    for(iter=name_confd.begin();iter!=name_confd.end();iter++)
    {
        if(iter->second == connetfd)
            name = iter->first;
    }
    close(connetfd);
    name_confd.erase(name);
    return 0;
}


int Server::getfd(string name)
{
    char name_[NAMESIZE];
    strcpy(name_,name.c_str());
    strcat(PATH_SERVER,name_);
    int fd = open(PATH_SERVER,O_CREAT|O_APPEND|O_RDWR);
    if(fd < 0)
    {
        log("getfd error");
        return -1;
    }
    return fd;
}

string Server::getstr(int fd,int line)
{
    int n;
    char buf[256];
    FILE* fp = fdopen(fd,"r");
    for(int i=0;i<line;i++)
        fgets(buf,256,fp);
    n = strlen(buf);
    if(buf[n-1] == '\n')
        buf[n-1] = '\0';
    return buf;
}


int Server::writestr(string openname,string writename)
{
    int fd,n;
    char name[NAMESIZE];
    strcpy(name,openname.c_str());
    strcat((char*)PATH_SERVER,name);
    fd = open(PATH_SERVER,O_RDWR|O_APPEND|O_CREAT);
    char name_[NAMESIZE];
    strcpy(name_,writename.c_str());
    strcat(name_,"\n");
    n = strlen(name_);
    if(write(fd,name_,n) != n)
    {
        log("writestr error");
        return -1;
    }
    return 0;
}

/********************main*****************************/

void* handle(void* arg);

int main(int argc,char** argv)
{
    Server server;
    int err;
//    Server* ser = &server;

    pthread_t tid;//以分离属性创建线程
    pthread_attr_t attr;
    err = pthread_attr_init(&attr);
    if(err!=0)
    {
        server.log("pthread_atte create error");
        return -1;
    }
    err = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    if(err!=0)
    {
        server.log("pthread_atte create error");
        return -1;
    }

    for( ; ;)
    {
        if(server.if_listenfd_read() == 0)//监听套接字可读
            err = pthread_create(&tid,&attr,handle,&server);
    }            

    exit(0);
}

void* handle(void* arg)
{
    Server server = (*(Server*)arg);
    int confd = server.create_connet();
    char name_[NAMESIZE];
    char password_[NAMESIZE];
    char buf[NAMESIZE];

    strcpy(buf,"login succeed!\n");
    strcpy(name_,"name: ");
    strcpy(password_,"password: ");
    writen(confd,name_,sizeof(name_));
    readn(confd,name_,NAMESIZE);
    writen(confd,password_,sizeof(password_));
    readn(confd,password_,NAMESIZE);
    writen(confd,buf,sizeof(buf));


    string name = name_;
    string password = password_;

    if(server.pty(confd,name,password) == 0)
    {
        server.send_online_friend_list(name,confd);
        server.handle_message(confd);
        if(server.empty_inline_message() != 0)
            server.handle_inline_message();
    }
    server.closefd(confd);
    pthread_exit(0);
}

/*void* handle_send(void* arg);
void* handle_receive(void* arg);

int main(int argc,char** argv)
{
    int err;
    string name,password;
    cout << "Please enter the login information: " << endl;
    cin >> name;
    cin >> password;
    Client client(name,password);
    client.show_online_friend_list();

    pthread_t tid;//线程及属性创建
    pthread_attr_t attr;
    err = pthread_attr_init(&attr);
    if(err!=0)
    {
        cerr << "pthread_create error" << endl;
        exit(-1);
    }
    err = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    if(err!=0)
    {
        cerr << "pthread_attr error" << endl;
        exit(0);
    }
    pthread_create(&tid,&attr,handle_send,&client);
    pthread_create(&tid,&attr,handle_receive,&client);

    exit(0);
}

void* handle_send(void* arg)
{
    Client client = (*(Client*)arg);
    string recvname,message;
    cout << "请在发消息时指定收信人姓名和内容。" << endl;
    cout << "请在结束时输入两个q。" << endl;
    for(;;)
    {
        cin >> recvname;
        cin >> message;
        if(recvname=="q" || message=="q")
            pthread_exit(0);
        client.send_message(recvname,message);
    }
}

void* handle_receive(void* arg)
{
    Client client = (*(Client*)arg);
    for(;;)
    {
        if(client.if_socketfd_read())
        {
            client.receive_message();
        }
    }
}*/

    






















    





