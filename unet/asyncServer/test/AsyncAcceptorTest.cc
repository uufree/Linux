/*************************************************************************
	> File Name: AsyncAcceptorTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月18日 星期二 19时04分19秒
 ************************************************************************/

#include<iostream>
#include"../AsyncAcceptor.h"
#include"../../InetAddress.h"

int main(int argc,char** argv)
{
    unet::net::socket::InetAddress inet(7777);
    unet::net::AsyncAcceptor accept(inet);

    return 0;
}

