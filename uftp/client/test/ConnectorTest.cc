/*************************************************************************
	> File Name: ConnectorTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月14日 星期五 09时48分31秒
 ************************************************************************/

#include"../Connector.h"

using namespace unet;
using namespace unet::net;
using namespace unet::thread;

int main(int argc,char** argv)
{
    InetAddress serveraddr(7777);
    Connector connector(&serveraddr);

    return 0;
}

