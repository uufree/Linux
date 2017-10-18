/*************************************************************************
	> File Name: EpollerTestClient.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月19日 星期三 15时01分58秒
 ************************************************************************/

#include<iostream>
#include"../Socket.h"
#include"../Buffer.h"

using namespace unet;

int main(int argc,char** argv)
{
    sleep(3);

    net::socket::InetAddress serverAddr("127.0.0.1",7777);
    net::socket::Socket confd(net::socket::CONNECT);
    net::socket::connect(confd,serverAddr);
     
    return 0;
}

