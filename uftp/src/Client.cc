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

//是为了一次传输多个文件设计，但是目前只能传输一个文件。
//在进行PUT操作时，我们希望您能提供本地文件的全局路径。

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
    client.connect(&addr);
/*    
    while(1)
    {
        client.sendMessage("hello,server!");
        sleep(1);
    }

*/    
    TranslateType type;  
    

    const char* json = "{\"type\":0,\"directory\":[]}";

    Document docu;
    StringBuffer buffer;
    docu.Parse(json);

    Value& s = docu["type"];
    Value& sp = docu["directory"];
    Document::AllocatorType& allocater = docu.GetAllocator();
    
    if(argc <= 1 )
    {
        printf("缺少命令!\n");
        exit(0);
    }
    else
    {
        if(strcmp(argv[1],"list") == 0)
        {
            s.SetInt(0);
            type = LIST;
        }
        else if(strcmp(argv[1],"get") == 0)
        {
            s.SetInt(1);
            type = GET;
        }
        else if(strcmp(argv[1],"put") == 0)
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
    
    if(argc == 3)//需要处理get的绝对路径和put的全局路径
    {
        Value va;
        char buf[64];
        char* ptr = NULL;
        
        if(strcmp(argv[1],"get") == 0)
        {
            for(int i=2;i<argc;++i)
            {
                va.SetString(StringRef(argv[i]));
                sp.PushBack(va,allocater);
                vec.push_back(argv[i]);
            }
        }
        else if(strcmp(argv[1],"put") == 0)
        {
            for(int i=2;i<argc;++i)
            {
                bzero(buf,64);
                ptr = strrchr(argv[i],'/');
                ptr++;
                strcpy(buf,ptr);

                va.SetString(StringRef(buf));
                sp.PushBack(va,allocater);
                vec.push_back(buf);
            }
        }
        else
        {
            std::cout << "抱歉，目前版本只能传输一个文件,我们鼓励您打包之后再传输~" << std::endl;
        }
    }     
    
    Writer<StringBuffer> writer(buffer);
    docu.Accept(writer);
    
    client.sendMessage(buffer.GetString());

    switch (type)
    {
        case LIST:
        {
            char buffer[2048];
            bzero(buffer,2048);
            client.recvMessage(buffer);
            std::cout << buffer;
            break;
        }
        case GET:
        {
            char path[64];
            for(unsigned int i=0;i<vec.size();++i)
            {
                bzero(path,64);
                getcwd(path,64);
                strcat(path,"/");
                strcat(path,vec[i].c_str());     
                
                client.recvFile(path);
            }
            break;
        }
        case PUT:
        {
            sleep(1);//无奈之举，不得不等Server的从第一次readInSocket中退出来,哎～～
            
            for(int i=2;i<argc;++i)
            {
                std::cout << argv[i] << std::endl;
                client.sendFile(argv[i]);
            }
            break;
        }
    }
    
    return 0;
}
