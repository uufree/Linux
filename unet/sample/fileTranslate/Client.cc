/*************************************************************************
	> File Name: Client.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月31日 星期五 16时28分42秒
 ************************************************************************/

//这个例子处理两种情况的事件

//服务端主动给多个客户端发送文件
//多个客户端主动给服务端发送文件

#include"../../client/TcpClient.h"
#include"../../ThreadPool.h"

using namespace unet;
using namespace unet::thread;
using namespace unet::net;

void readCallBack(Buffer* inputbuffer,Buffer* outputbuffer)
{//被动的接受服务端传送的文件
    char buf[128];
    char buf1[32];
    bzero(buf,128);
    strcat(buf,"/home/uuchen/chenuu/");

    snprintf(buf1,32,"%ld",pthread_self());
    strcat(buf,buf1);
    strcat(buf,".jpeg");
    
    outputbuffer->recvFile(buf);
}

void writeCallBack(Buffer* inputbuffer,Buffer* outputbuffer)
{
}

void drivedCallBack(Buffer* inputbuffer,Buffer* outputbuffer)
{//服务端主动发送文件，及时服务
//    inputbuffer->sendFile("/home/uuchen/uuchen.jpeg");
}

int main(int argc,char** argv)
{
    InetAddress addr("127.0.0.1",7777);
    TcpClient client;//主动处理事件的能力发生在client对象的构建过程中。server对象可以，但是client不行
    
    client.setReadCallBack(std::bind(&readCallBack,std::placeholders::_1,std::placeholders::_2));
    client.setWriteCallBack(std::bind(&writeCallBack,std::placeholders::_1,std::placeholders::_2));
    client.setDrivedCallBack(std::bind(&drivedCallBack,std::placeholders::_1,std::placeholders::_2));
    client.start();
   
    client.connection(&addr);
    
    while(1)
    {
        //应用层的GUI代码
    }
    
    return 0;
}

