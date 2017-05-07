/*************************************************************************
	> File Name: Server.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月31日 星期五 16时28分26秒
 ************************************************************************/

//这个例子处理两种情况的事件

//服务端主动给多个客户端发送文件
//多个客户端主动给服务端发送文件


#include"../mutilServer/MutilTcpServer.h"
#include"../File.h"
#include"../rapidjson/document.h"
#include"../rapidjson/writer.h"
#include"../rapidjson/stringbuffer.h"

using namespace unet;
using namespace unet::thread;
using namespace unet::net;
using namespace rapidjson;

enum TransType{LIST,GET,PUT};
static unet::Directory dir("/home/uuchen/unet/");

void readCallBack(Buffer* inputbuffer,Buffer* outputbuffer)
{//服务端被动处理事务（适用于短连接）,只能在fd资源和内存资源中二选一
    char stream[1024];
    outputbuffer->readInSocket();
    outputbuffer->getCompleteMessageInBuffer(stream);
    
    Document docu;
    docu.Parse(stream);
    int type = docu["type"].GetInt();
     
    switch (type)
    {
        case LIST:
        {
            inputbuffer->appendInBuffer(dir.getDirBuffer());
            inputbuffer->writeInSocket();
            break;
        }
        case GET:
        {
            char path[64];
            bzero(path,64);
            strcat(path,"/home/uuchen/unet/");
            Value& sp = docu["directory"];
            for(unsigned int i=0;i<sp.Size();++i)
            {
                if(dir.inDirectoryList(sp[i].GetString()))
                {
                    strcat(path,sp[i].GetString());
                    inputbuffer->sendFile(path);
                }
            }
            break;
        }
        case PUT:
        {
            char path[64];
            bzero(path,64);
            strcat(path,"/home/uuchen/unet/");
            Value& sp = docu["directory"];
            for(unsigned int i=0;i<sp.Size();++i)
            {
                if(!dir.inDirectoryList(sp[i].GetString()))
                {
                    strcat(path,sp[i].GetString());
                    inputbuffer->recvFile(path);
                }
            }
            break;
        }
    }
}

void writeCallBack(Buffer* inputbuffer,Buffer* outputbuffer)
{
}

void drivedCallBack(Buffer* inputbuffer,Buffer* outputbuffer)
{//服务端主动发送文件，及时服务
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



