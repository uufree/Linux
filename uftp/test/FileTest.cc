/*************************************************************************
	> File Name: FileTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月10日 星期一 11时15分58秒
 ************************************************************************/

#include<iostream>
#include"../File.h"

using namespace unet;

int main(int argc,char** argv)
{
    File uuchen("uuchen");
    char buffer[256];
    read(uuchen.getFd(),buffer,256);
    std::cout << buffer << std::endl;
    return 0;
}
