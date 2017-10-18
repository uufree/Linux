/*************************************************************************
	> File Name: TimestampTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月27日 星期四 13时52分00秒
 ************************************************************************/

#include<iostream>
#include"../Timestamp.h"

int main(int argc,char** argv)
{
    unet::time::Timestamp time;
    std::cout << time.getTime() << std::endl;
    return 0;
}
