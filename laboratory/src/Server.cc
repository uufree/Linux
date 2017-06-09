/*************************************************************************
	> File Name: Server.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月31日 星期五 16时28分26秒
 ************************************************************************/

#include"../mutilServer/MutilTcpServer.h"
#include"../File.h"

using namespace unet;
using namespace unet::thread;
using namespace unet::net;

void readCallBack(Buffer* inputbuffer,Buffer* outputbuffer)
{//服务端被动处理事务（适用于短连接）,只能在fd资源和内存资源中二选一
    char stream[1024];
    bzero(stream,1024);

    while(1)
    {
        if(outputbuffer->readInSocket() != 0)
        {
            outputbuffer->getCompleteMessageInBuffer(stream);
            std::cout << stream << std::endl;
        }
    }   
}

void writeCallBack(Buffer* inputbuffer,Buffer* outputbuffer)
{
}

void drivedCallBack(Buffer* inputbuffer,Buffer* outputbuffer)
{
}


int main(int argc,char** argv)
{
    InetAddress serveraddr(7777);
    MutilTcpServer server(&serveraddr,1);

    server.setReadCallBack(std::bind(&readCallBack,std::placeholders::_1,std::placeholders::_2));
    server.setWriteCallBack(std::bind(&writeCallBack,std::placeholders::_1,std::placeholders::_2));
    server.setDrivedCallBack(std::bind(&drivedCallBack,std::placeholders::_1,std::placeholders::_2));

    server.start();

    return 0;
}

