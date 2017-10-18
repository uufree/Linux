/*************************************************************************
	> File Name: ThreadPoolTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月12日 星期三 20时46分06秒
 ************************************************************************/

#include<iostream>
#include"../Thread.h"
#include"../Mutex.h"
#include"../Condition.h"
#include"../ThreadPool.h"
#include<unistd.h>

using namespace unet::thread;

void threadFunc()
{
    while(1)
    {
        sleep(1);
        std::cout << "I'm thread!" << std::endl;
    }
}



int main(int argc,char** argv)
{
    ThreadPool pool(4);
    pool.setThreadCallBack(std::bind(&threadFunc));
    pool.start();
    
    while(1)
    {
        sleep(1);
        std::cout << "I'm main!" << std::endl;
    }

    pool.joinAll();
    return 0;
}


