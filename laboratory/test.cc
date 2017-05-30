/*************************************************************************
	> File Name: test.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月30日 星期二 11时23分16秒
 ************************************************************************/

#include"SerialPort.h"
#include<iostream>
#include<unistd.h>
#include<sys/ioctl.h>
#include<stdlib.h>

using namespace serial;

int kbhit()
{
    int i=0;
    ioctl(0,FIONREAD,&i);
    return i;
}

int main(int argc,char** argv)
{
    SerialPort port("/dev/tty");
    port.printMessage();
    
    system("stty raw -echo");
    
    char c = ' '; 
    int i=0;
    for(;c!='q';i++)
    {
        if(kbhit())
        {
            c = std::cin.get();
            port.write(c);
        }
    }
    system("stty cooked echo");

    return 0;
}

