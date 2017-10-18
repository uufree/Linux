/*************************************************************************
	> File Name: FileTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月08日 星期六 13时45分13秒
 ************************************************************************/

#include"../File.h"
#include<iostream>

using namespace unet::file;

int main(int argc,char** argv)
{
    std::string mess;
    std::string message("I'm uuchen!");
    File uuchen("/home/uuchen/uuchen.txt",N_WRITE);
    
    unet::file::writen(uuchen.getFd(),message);

    File chenuu = std::move(uuchen);
    
    std::cout << "fd: " << chenuu.getFd() << std::endl;
    std::cout << "g_filename: " << chenuu.getGlobalFilename() << std::endl;
    std::cout << "filename: " << chenuu.getFilename() << std::endl;
    
    readn(chenuu.getFd(),mess,256);
    std::cout << "finish readn" << std::endl;
    std::cout << mess << std::endl;
    return 0;
}

