/*************************************************************************
	> File Name: ThreadPoolTest1.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月07日 星期日 11时17分43秒
 ************************************************************************/
//测试线程池的任务队列

#include"../ThreadPool.h"
#include<iostream>

typedef std::function<void()> Task;

void print(thread::ThreadPool* pool)
{
    Task task;
    while(1)
    {
        task = pool->getTaskInTaskQueue();
        task();
        sleep(1);
    }
}

void fs(int num)
{
    std::cout << ::pthread_self() << "--" << num << std::endl;
}


int main(int argc,char** argv)
{

    thread::ThreadPool* pool = new thread::ThreadPool(2);
    
    pool->setThreadCallBack(std::bind(&print,pool));

    pool->start();

    for(int i=0;i<10;++i)
    {
        pool->addInTaskQueue(std::bind(&fs,i));
        sleep(1);
    }

    pool->joinAll();

    std::cout << "sleep~" << std::endl;
    sleep(2);

    return 0;
}



