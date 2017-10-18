/*************************************************************************
	> File Name: SocketAndInetTestServer.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月11日 星期二 11时15分48秒
 ************************************************************************/

#include<iostream>
#include"../Socket.h"
#include"../InetAddress.h"
#include"../File.h"

using namespace unet::net;

int main(int argc,char** argv)
{
    socket::InetAddress serverAddr(7777);
    socket::Socket listenfd(socket::LISTEN); 
    socket::bind(listenfd,serverAddr);
    socket::listen(listenfd);

    int confd = socket::accept(listenfd);
    std::cout << "confd: " << confd << std::endl;
    
    socket::Socket connectfd(confd);
    std::string messageClient("hello,client!");
    std::string messageServer;
   
    char buffer[32];
    bzero(buffer,32);

    while(1)
    {
        unet::file::writen(connectfd.getFd(),messageClient);
        
        read(connectfd.getFd(),buffer,32);
        std::cout << buffer << std::endl;
        
        sleep(1);
        bzero(buffer,32);
    }

    return 0;
}
