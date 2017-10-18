/*************************************************************************
	> File Name: EpollerTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月18日 星期二 16时28分13秒
 ************************************************************************/

#include<iostream>
#include"../Epoller.h"

int main(int argc,char** argv)
{
    unet::net::EventList eventList;
    unet::net::Epoller Epoller(eventList);

    return 0;
}

