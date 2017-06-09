/*************************************************************************
	> File Name: test.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月09日 星期五 16时16分20秒
 ************************************************************************/

#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<string.h>

int kbhit()
{
    int i=0;
    ioctl(0,FIONREAD,&i);
    return i;
}

int main(int argc,char** argv)
{
    system("stty raw -echo");
    
    char c = ' '; 
    int i=0;
    for(;c!='q';i++)
    {
        if(kbhit())
        {
            c = std::cin.get();
            std::cout << c << std::endl;
        }
    }
    system("stty cooked echo");
    
    return 0;
}

