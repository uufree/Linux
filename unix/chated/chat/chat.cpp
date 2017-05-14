/**********************************************************************
	> File Name: chat.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月20日 星期二 03时08分26秒
 *********************************************************************/

#include"chat.h"
using namespace std;
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

ssize_t writen(int fd,void* vptr,size_t n)
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

/*********************client类方法*************************/
Client::Client(string name_,string password_):name(name_),password(password_)
{
    socketfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,"127.0.0.1",&serveraddr.sin_addr);
    
    char buf_[NAMESIZE];
    if(connect(socketfd,(sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
        cerr << "connect error" << endl;

    strcpy(buf_,name.c_str());
    writen(socketfd,buf_,NAMESIZE);
    strcpy(buf_,password.c_str());
    writen(socketfd,buf_,NAMESIZE);
    
/*    int fd,n;//初始化好友列表
    char buf[MAXLINE];
    FILE* fp;
    string posname;
    char name__[NAMESIZE];
    strcpy(name__,name.c_str());
    strcat(PATH_CLIENT,name__);
    fd = open(PATH_CLIENT,O_CREAT|O_APPEND|O_RDWR);
    if((fp=fdopen(fd,"r")) < 0)
        cerr << "fp open error" << endl;
    for(;;)
    {
        if(fgets(buf,256,fp) == NULL)
            break;
        if(buf != NULL)
            n = strlen(buf);
        else
            break;
        if(buf[n-1] == '\n')
            buf[n-1] = '\0';
        posname = buf;
        friend_list.push_back(posname);
    }*/

/*  struct online_list list;
    readn(socketfd,&list,sizeof(list));//初始化在线好友列表
    string plname;
    for(int i=0;i<25;i++)
    {
        if(list.name[i] != NULL)
        {
            plname = list.name[i];
            online_friend_list.push_back(plname);
        }
    }   */
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
}

/**********************server类方法***********************/
Server::Server()
{//实例化对象后，监听套接字一直存在
    struct sockaddr_in serveraddr;
    listenfd = socket(AF_INET,SOCK_STREAM,0);

    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
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

