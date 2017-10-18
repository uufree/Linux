/*************************************************************************
	> File Name: SocketAndInetTestClient.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月11日 星期二 11时15分28秒
 ************************************************************************/

#include<iostream>
#include"../Socket.h"
#include"../InetAddress.h"
#include"../File.h"

using namespace unet::net;

int main(int argc,char** argv)
{
    socket::InetAddress serverAddr("127.0.0.1",7777);
    socket::Socket connectfd(socket::CONNECT);
    socket::connect(connectfd,serverAddr);

    std::string messageServer("hello,server!");
    std::string messageClient;
    
    char buffer[32];
    bzero(buffer,32);

    while(1)
    {
        read(connectfd.getFd(),buffer,32);
        std::cout << buffer << std::endl;
        
        unet::file::writen(connectfd.getFd(),messageServer);
        
        sleep(1);
        bzero(buffer,32);
    }

    return 0;
}


