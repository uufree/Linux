/*************************************************************************
	> File Name: test.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月03日 星期六 21时30分15秒
 ************************************************************************/

#include<iostream>
#include"File.h"

using namespace unet;

int main(int argc,char** argv)
{
    char buf[512];
    File file("/home/uuchen/git/uftp/weather.csv");
    file.readn(buf,512);
    std::cout << buf << std::endl;
    return 0;
}

