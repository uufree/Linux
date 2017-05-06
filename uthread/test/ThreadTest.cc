/*************************************************************************
	> File Name: ThreadTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月06日 星期六 17时55分37秒
 ************************************************************************/

#include<iostream>
#include"../Thread.h"
#include<unistd.h>

void printInt(int num)
{
    for(int i=0;i<10;++i)
    {
        std::cout << "num: " << num << std::endl;
        ::sleep(1);
    }
}

int main(int argc,char** argv)
{
    int n = 5;
    thread::Thread thread(std::bind(&printInt,n));
    thread.start();
    
    sleep(3);
    
    thread.exit();
    std::cout << "thread already stop!" << std::endl;

    sleep(2);

    return 0;
}
    
