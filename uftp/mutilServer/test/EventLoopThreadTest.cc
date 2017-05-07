/*************************************************************************
	> File Name: EventLoopThreadTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月28日 星期二 23时48分15秒
 ************************************************************************/

#include"../EventLoopThread.h"

using namespace unet;
using namespace unet::thread;

int main(int argc,char** argv)
{
    EventLoopThread* threads = new EventLoopThread[4];
    
    for(int i=0;i<4;++i)
        threads[i].start();

    sleep(10);
    return 0;
}

