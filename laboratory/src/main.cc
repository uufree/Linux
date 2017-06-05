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

