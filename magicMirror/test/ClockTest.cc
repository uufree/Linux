/*************************************************************************
	> File Name: ClockTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年08月17日 星期四 21时30分22秒
 ************************************************************************/

#include<iostream>
#include"../Clock.h"

int main(int argc,char** argv)
{
    utime::UClock clock;
    clock.update();

    std::cout << "getMouthAndDays: " << clock.getMouthAndDays() << std::endl;
    std::cout << "getHourAndMinutes: " << clock.getHourAndMinutes() << std::endl;
    
    return 0;
}

