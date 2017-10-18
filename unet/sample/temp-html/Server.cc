/*************************************************************************
	> File Name: Server.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月15日 星期六 17时12分40秒
 ************************************************************************/

//#include"../../asyncServer/AsyncTcpServer.h"
#include"../../mutilServer/MutilTcpServer.h"
#include"../../rapidjson/document.h"
#include"../../rapidjson/writer.h"
#include"../../rapidjson/stringbuffer.h"
#include"../../rapidjson/filereadstream.h"

using namespace unet::net;
using namespace unet::thread;
using namespace rapidjson;

void readCallBack(Buffer* inputbuffer,Buffer* outputbuffer)
{   
    char message[32];
    bzero(message,32);

//如果是用异步的server，删除掉readInSocket()    
    inputbuffer->readInSocket();
    inputbuffer->getCompleteMessageInBuffer(message);
//    std::cout << message << std::endl;
    
    if(message != nullptr)
    {
        Document docu;
        docu.Parse(message);
        Value& s = docu["temp"];
        printf("%s\n",message);
        printf("temp: %d\n",s.GetInt());
        bzero(message,32);
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
//    unet::net::AsyncTcpServer server(&serveraddr,2);
    unet::net::MutilTcpServer server(&serveraddr,2); 
    
    server.setReadCallBack(std::bind(&readCallBack,std::placeholders::_1,std::placeholders::_2));
    server.setWriteCallBack(std::bind(&writeCallBack,std::placeholders::_1,std::placeholders::_2));
    server.setDrivedCallBack(std::bind(&drivedCallBack,std::placeholders::_1,std::placeholders::_2));

    server.start();
    return 0;
}

