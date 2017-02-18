/*********************************************************************
  > File Name: server.cpp
  > Author: UUChen
  > Mail: 18149366134@163.com
  > Created Time: 2016年09月23日 星期五 00时08分25秒
 ********************************************************************/

#include"chat.h"
#include"chat.cpp"
using namespace std;

int main(int argc,char** argv)
{   
    Server server;
    server.setpretendfd();
    ThreadPoll threadpoll;

    threadpoll.Start();

    while(1)
    {
        if(server.if_listenfd_read())
            server.create_connet();//第二次登录卡在了这一步
    }

    exit(0);
}
