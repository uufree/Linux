/**********************************************************************
	> File Name: client.cpp
	> Author:UUChen 
	> Mail: 18149366134@163.com
	> Created Time: 2016年09月28日 星期三 01时10分35秒
**********************************************************************/

#include"chat.h"
#include"chat.cpp"
using namespace std;

void* handle_send(void* arg);//接受消息线程
void* handle_receive(void* arg);//发送消息线程

int main(int argc,char** argv)//写的比较随意，剩下的就是处理业务逻辑
{
    int err;
    string name,password;
    cout << "name: ";
    cin >> name;
    cout << "password: ";
    cin >> password;
    Client client(name,password);

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
    
    sleep(1200);//主线程没事做。。休眠吧。。哈哈

    exit(0);
}

void* handle_send(void* arg)
{
    Client client = (*(Client*)arg);//处理传递过程
    string recvname,message;
    for(;;)//终止消息处理
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
    Client client = (*(Client*)arg);//处理传递过程
    for(;;)
            client.receive_message();
}

    
