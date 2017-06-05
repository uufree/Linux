/*************************************************************************
	> File Name: MutilAcceptorTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月30日 星期四 00时00分09秒
 ************************************************************************/

#include"../MutilAcceptor.h"

using namespace unet;
using namespace unet::net;
using namespace unet::thread;

int main(int argc,char** argv)
{
    InetAddress addr(7777);
    MutilAcceptor acceptor(&addr);

    return 0;
}

