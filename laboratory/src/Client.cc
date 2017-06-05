/*************************************************************************
	> File Name: Client.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月31日 星期五 16时28分42秒
 ************************************************************************/

#include"../singleClient/SingleTcpClient.h"
#include"../rapidjson/document.h"
#include"../rapidjson/writer.h"
#include"../rapidjson/stringbuffer.h"
#include<vector>
#include<string>
#include<string.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<stdlib.h>

enum TranslateType{LIST,GET,PUT};

using namespace unet;
using namespace unet::thread;
using namespace unet::net;
using namespace rapidjson;

int kbhit()
{
    int i=0;
    ioctl(0,FIONREAD,&i);
    return i;
}

int main(int argc,char** argv)
{
    InetAddress addr("120.25.0.40",7777);
    SingleTcpClient client;//主动处理事件的能力发生在client对象的构建过程中。server对象可以，但是client不行
    client.connect(&addr);
     
    system("stty raw -echo");
    
    char c = ' '; 
    int i=0;
    for(;c!='q';i++)
    {
        if(kbhit())
        {
            c = std::cin.get();
            std::cout << c << std::endl;
            client.sendMessage(&c);
        }
    }
    system("stty cooked echo");

    return 0;
}
