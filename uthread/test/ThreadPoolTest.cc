/*************************************************************************
	> File Name: ThreadPoolTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月06日 星期六 22时43分01秒
 ************************************************************************/

//测试线程池的线程功能

#include"../ThreadPool.h"
#include<iostream>

void print()
{
    for(int i=0;i<5;++i)
    {
        std::cout << ::pthread_self() << std::endl;
        sleep(1);
    }
}


int main(int argc,char** argv)
{

    thread::ThreadPool pool(2);
    pool.setThreadCallBack(std::bind(&print));
    
    thread::ThreadPool pool_(std::move(pool));

    pool_.start();
    pool_.joinAll();

    std::cout << "sleep~" << std::endl;
    sleep(2);

    return 0;
}



