/*************************************************************************
	> File Name: Client.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月15日 星期六 17时12分46秒
 ************************************************************************/

#include"../../Buffer.h"
#include"../../Socket.h"
#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>

using namespace unet;

int main(int argc,char** argv)
{
    net::socket::InetAddress server("192.168.1.106",6666);
    net::socket::Socket confd(net::socket::CONNECT);
    net::socket::connect(confd,server);
    
    std::string message("hello,server!");
    net::Buffer buffer(confd.getFd());
    buffer.appendInBuffer(message);
    buffer.writeInSocket();

    return 0;
}










