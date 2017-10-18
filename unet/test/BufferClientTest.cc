/*************************************************************************
	> File Name: BufferClientTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月15日 星期六 19时36分18秒
 ************************************************************************/

#include<iostream>
#include"../Socket.h"
#include"../Buffer.h"

using namespace unet;

int main(int argc,char** argv)
{
    net::socket::InetAddress serverAddr("127.0.0.1",7777);
    net::socket::Socket confd(net::socket::CONNECT);
    net::socket::connect(confd,serverAddr);
    
    std::cout << "confd is: " << confd.getFd() << std::endl;

    net::Buffer clientBuffer(confd.getFd());
    
    clientBuffer.sendFile("/home/uuchen/uuchen.jpeg");
    
    return 0;
}
