/*************************************************************************
	> File Name: BufferTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月15日 星期六 19时54分51秒
 ************************************************************************/

//测试Buffer的安全性，联网测试另见

#include<iostream>
#include"../Buffer.h"

using namespace unet::net;

int main(int argc,char** argv)
{
    std::string message;
    std::string message1("hello,world!");//12
    std::string message2("hello,FLY");//9
    std::string message3("hello,DXJ");//9
    Buffer buffer(10);

    buffer.appendInBuffer(message1);
    buffer.appendInBuffer(message2);
    buffer.appendInBuffer(message3);
        
    buffer.getCompleteMessageInBuffer(message);
    std::cout << "message: " << message << std::endl;
    buffer.getCompleteMessageInBuffer(message);
    std::cout << "message: " << message << std::endl;
    buffer.getCompleteMessageInBuffer(message);
    std::cout << "message: " << message << std::endl;

    return 0;
}

