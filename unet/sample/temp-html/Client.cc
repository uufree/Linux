/*************************************************************************
	> File Name: Client.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月15日 星期六 17时12分46秒
 ************************************************************************/

#include"../../client/TcpClient.h"
#include"../../rapidjson/document.h"
#include"../../rapidjson/writer.h"
#include"../../rapidjson/stringbuffer.h"
#include"../../rapidjson/filereadstream.h"

using namespace unet::net;
using namespace unet::thread;
using namespace unet::thread;
using namespace rapidjson;

void readCallBack(Buffer* inputbuffer,Buffer* outputbuffer)
{
}

void writeCallBack(Buffer* inputbuffer,Buffer* outputbuffer)
{

}

void drivedCallBack(Buffer* inputbuffer,Buffer* outputbuffer)
{

}

int main(int argc,char** argv)
{
    InetAddress serveraddr("127.0.0.1",7777);
    TcpClient client;

    client.setReadCallBack(std::bind(&readCallBack,std::placeholders::_1,std::placeholders::_2));
    client.setWriteCallBack(std::bind(&writeCallBack,std::placeholders::_1,std::placeholders::_2));
    client.setDrivedCallBack(std::bind(&drivedCallBack,std::placeholders::_1,std::placeholders::_2));
    
    client.start();
    
    char buf[256];
    bzero(buf,256);
    FILE* fp = fopen("/home/uuchen/unet/sample/temp-html/temp.json","rb");
    FileReadStream is(fp,buf,sizeof(buf));   
    Document docu;
    docu.ParseStream(is);
    
    StringBuffer buffer;
 
    client.connection(&serveraddr);
    
    for(int i=0;i<5;++i)
    {
        
        Value& s = docu["temp"];
        s.SetInt(s.GetInt()+1);
        
        buffer.Clear();
        Writer<StringBuffer> writer(buffer);
        docu.Accept(writer);        
        client.writeInAsyncBuffer(buffer.GetString());

//        client.writeInAsyncBuffer("hello,server!");
        sleep(1);
    }
    
    client.closeConnection();
    
    fclose(fp);
       
    return 0;
}









