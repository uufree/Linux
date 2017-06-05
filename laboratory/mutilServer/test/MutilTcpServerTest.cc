/*************************************************************************
	> File Name: MutilTcpServerTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月29日 星期三 23时56分51秒
 ************************************************************************/

#include"../MutilTcpServer.h"

int main(int argc,char** argv)
{
    unet::net::InetAddress serveraddr(7777);
    
    unet::net::MutilTcpServer server(&serveraddr);

    server.start();
    
    return 0;
}

