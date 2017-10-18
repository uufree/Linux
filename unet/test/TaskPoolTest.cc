/*************************************************************************
	> File Name: TaskPoolTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月18日 星期二 19时11分21秒
 ************************************************************************/

#include<iostream>
#include"../TaskPool.h"

int main(int argc,char** argv)
{
    unet::net::TcpConnectionMap map;
    unet::thread::TaskPool pool(2,map); 

    return 0;
}

