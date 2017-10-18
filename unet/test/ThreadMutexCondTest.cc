/*************************************************************************
	> File Name: ThreadTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月12日 星期三 20时08分05秒
 ************************************************************************/

#include<iostream>
#include"../Thread.h"
#include<unistd.h>
#include"../Condition.h"

using namespace unet::thread;

unet::thread::MutexLock lock;
unet::thread::Condition cond(lock);
int m = 0;

void threadFunc()
{
    MutexLockGuard guard(lock);
    while(m < 10)
    {
        cond.wait();
    }
    guard.~MutexLockGuard();

    while(1)
    {
        MutexLockGuard guard(lock);
        sleep(1);
        ++m;
        std::cout << "pid: " << unet::now::pid() << std::endl << m << std::endl;
    }
}


int main(int argc,char** argv)
{
    Thread thread;
    thread.setThreadCallBack(std::bind(&threadFunc));

    thread.start();
    
    while(1)
    {
        if(m < 10)
        {
            MutexLockGuard guard(lock);
            ++m;
            sleep(1);
            std::cout << "pid: " << unet::now::pid() << std::endl << m << std::endl;
        }
        else
            break;    
    }

    cond.notify();

    thread.join();

    return 0;
}
