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

enum TranslateType{LIST,GET,PUT};

using namespace unet;
using namespace unet::thread;
using namespace unet::net;
using namespace rapidjson;

int main(int argc,char** argv)
{
    InetAddress addr("127.0.0.1",7777);
    SingleTcpClient client;//主动处理事件的能力发生在client对象的构建过程中。server对象可以，但是client不行
    std::vector<std::string> vec;
    TranslateType type;  

    client.connect(&addr);
    const char* json = "{\"type\":0,\"directory\":[]}";
    
    Document docu;
    StringBuffer buffer;
    docu.Parse(json);

    Value& s = docu["type"];
    Value& sp = docu["directory"];
    Document::AllocatorType& allocater = docu.GetAllocator();
    
    if(argc < 1)
    {
        printf("缺少命令!\n");
        exit(0);
    }
    else
    {
        if(strcmp(argv[1],"LIST") == 0)
        {
            s.SetInt(0);
            type = LIST;
        }
        else if(strcmp(argv[1],"GET") == 0)
        {
            s.SetInt(1);
            type = GET;
        }
        else if(strcmp(argv[1],"PUT") == 0)
        {
            s.SetInt(2);
            type = PUT;
        }
        else
        {
            printf("命令错误!\n");
            exit(0);
        }
    }
    
    if(argc >= 2)
    {
        for(int i=2;i<argc;++i)
        {
            sp.PushBack(static_cast<std::string>(argv[i]),allocater);
            vec.push_back(argv[i]);
        }
    }     
    
    char sendBuffer[1024];
    bzero(sendBuffer,1024);
    Writer<StringBuffer> writer(buffer);
    docu.Accept(writer);
    
    client.sendMessage(sendBuffer);
    
    switch (type)
    {
        case LIST:
        {
            char buffer[2048];
            client.recvMessage(buffer);
            std::string str(buffer);
            for(size_t i=0;i<str.size();++i)
            {
                if(str[i] == '\t')
                    str[i] = '\n';
            }
            std::cout << str;
            break;
        }
        case GET:
        {
            char path[64];
            for(size_t i=0;i<vec.size();++i)
            {
                bzero(path,64);
                getcwd(path,64);
                strcat(path,vec[i].c_str());     
                
                client.recvFile(path);
            }
        }
        case PUT:
        {
            for(size_t i=0;i<vec.size();++i)
                client.recvFile(vec[i].c_str());
        }
    }

    return 0;
}

