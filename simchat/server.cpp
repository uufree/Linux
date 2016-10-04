/*********************************************************************
  > File Name: server.cpp
  > Author: UUChen
  > Mail: 18149366134@163.com
  > Created Time: 2016年09月23日 星期五 00时08分25秒
 ********************************************************************/

#include"chat.h"
#include"chat.cpp"
using namespace std;

void* handle(void* arg);//线程处理函数

int main(int argc,char** argv)
{
    Server server;//创建服务器对象
    int err;

    pthread_t tid;//以分离属性创建线程
    pthread_attr_t attr;//线程属性
    err = pthread_attr_init(&attr);
    if(err!=0)
    {
        cerr << "pthread attr create error" << endl;
        return -1;
    }
    err = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    if(err!=0)
    {
        cerr << "pthread_atte create error" << endl;
        return -1;
    }

    pthread_mutex_init(&lock,NULL);//初始化线程锁
    for( ; ;)
    {
        if(server.if_listenfd_read() == 0)//监听套接字可读
            err = pthread_create(&tid,&attr,handle,&server);
    }            
    pthread_mutex_destroy(&lock);//删除线程锁
    exit(0);
}

void* handle(void* arg)
{
    int fd;
    string name,message;
    Server server = (*(Server*)arg);//相当于自己创建里一个新对象

    fd = server.create_connet();//创建与服务器的连接
    for(;;)
        server.handle_message(fd);//处理消息

    pthread_exit(0);//线程终止
}

