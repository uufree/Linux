/*************************************************************************
	> File Name: TimerEventSystemTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年08月16日 星期三 22时02分13秒
 ************************************************************************/

#include<iostream>
#include"../TimerEventSystem.h"

void timeReadCallBack()
{
    std::cout << "Timer was locked!" << std::endl;
}


int main(int argc,char** argv)
{
    unet::net::TimerEventSystem system;
    
    unet::time::TimerPtr timer(new unet::time::Timer(true,1));
    timer->setTimeCallBack(std::bind(&timeReadCallBack));
    system.addTimer(std::move(timer));
    
    system.start();
    return 0;
}
