/*************************************************************************
	> File Name: Server.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月31日 星期五 16时28分26秒
 ************************************************************************/

#include"../singleServer/TcpServer.h"

using namespace unet;
using namespace unet::thread;
using namespace unet::net;

int main(int argc,char** argv)
{
    InetAddress serveraddr(7777);
    TcpServer server(&serveraddr);
        
    server.loop();

    return 0;
}

