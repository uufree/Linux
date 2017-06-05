/*************************************************************************
	> File Name: dirtest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月07日 星期日 19时50分28秒
 ************************************************************************/

#include"../File.h"
#include<iostream>

int main(int argc,char** argv)
{
    unet::Directory dir("/home/uuchen/unet/");
    std::cout << dir.getDirBuffer() << std::endl;
    return 0;
}

