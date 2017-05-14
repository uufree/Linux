/*********************************************************************
  > File Name: server.cpp
  > Author: 
  > Mail: 
  > Created Time: 2016年09月23日 星期五 00时08分25秒
 ********************************************************************/

#include"chat.h"
#include"chat.cpp"//加上这个
using namespace std;

void* handle(void* arg);

int main(int argc,char** argv)
{
    Server server;
    int err;

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
    char buf[NAMESIZE];
    char name_[NAMESIZE];
    char password_[NAMESIZE];

    strcpy(buf,"login succeed!\n");
    readn(confd,name_,NAMESIZE);
    cout << name_ << endl;
    readn(confd,password_,NAMESIZE);
    cout << password_ << endl;
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

