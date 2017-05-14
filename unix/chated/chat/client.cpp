/**********************************************************************
	> File Name: client.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月28日 星期三 01时10分35秒
**********************************************************************/

#include"chat.h"
#include"chat.cpp"
using namespace std;

void* handle_send(void* arg);
void* handle_receive(void* arg);

int main(int argc,char** argv)
{
    int err;
    string name,password;
    cout << "name: ";
    cin >> name;
    cout << "password: ";
    cin >> password;
    Client client(name,password);
    cout << "connet success" << endl;
//    client.show_online_friend_list();

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
    
    sleep(120);

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
}

    
