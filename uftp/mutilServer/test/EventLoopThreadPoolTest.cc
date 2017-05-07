/*************************************************************************
	> File Name: EventLoopThreadPoolTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月29日 星期三 00时05分36秒
 ************************************************************************/

#include"../EventLoopThreadPool.h"

using namespace unet;
using namespace unet::thread;

int main(int argc,char** argv)
{
    EventLoopThreadPool pool;
    
    pool.start();
    
    sleep(10);
    return 0;
}


