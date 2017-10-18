/*************************************************************************
	> File Name: DirectoryTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月11日 星期二 10时29分49秒
 ************************************************************************/

#include<iostream>
#include"../Directory.h"

int main(int argc,char** argv)
{
    unet::file::Directory dir("/home/uuchen/unet");
    
    std::cout << dir.getDirBuffer() << std::endl;
    return 0;
}
