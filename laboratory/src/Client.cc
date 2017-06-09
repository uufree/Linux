/*************************************************************************
	> File Name: Client.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月31日 星期五 16时28分42秒
 ************************************************************************/

#include"../singleClient/SingleTcpClient.h"
#include<string.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<stdlib.h>

using namespace unet;
using namespace unet::thread;
using namespace unet::net;

int kbhit()
{
    int i=0;
    ioctl(0,FIONREAD,&i);
    return i;
}

int main(int argc,char** argv)
{
    InetAddress addr("127.0.0.1",7777);
    SingleTcpClient client;//主动处理事件的能力发生在client对象的构建过程中。server对象可以，但是client不行
    client.connect(&addr);
/*     
    system("stty raw -echo");
    
    char c = ' '; 
    int i=0;
    for(;c!='q';i++)
    {
        if(kbhit())
        {
            c = std::cin.get();
            std::cout << c;
            client.sendMessage(&c,1);
        }
    }
    system("stty cooked echo");
*/

    for(int i=0;i<5;++i)
    {
        client.sendMessage("hello,world",11);
        sleep(1);
    }
    
    std::cout << "leave circle!" << std::endl;
    return 0;
}
