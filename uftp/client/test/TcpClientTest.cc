/*************************************************************************
	> File Name: TcpClientTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月14日 星期五 15时18分53秒
 ************************************************************************/

#include"../TcpClient.h"

using namespace unet::net;
using namespace unet::thread;
using namespace unet;

int main(int argc,char** argv)
{
    InetAddress serveraddr(7777);
    TcpClient client(&serveraddr);
    
    client.start();
    

    sleep(5);
    return 0;
}


