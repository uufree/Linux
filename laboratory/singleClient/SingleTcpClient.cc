/*************************************************************************
	> File Name: SingleTcpClient.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月07日 星期日 21时29分43秒
 ************************************************************************/

#include"SingleTcpClient.h"
#include"../Buffer.h"

enum TransType{LIST,GET,PUT};

namespace unet
{
    namespace net
    {   
        SingleTcpClient::SingleTcpClient() : inputbuffer(0),outputbuffer(0)
        {};

        SingleTcpClient::~SingleTcpClient()
        {
            ::close(confd);
        };

        void SingleTcpClient::connect(InetAddress* addr_)
        {
            confd = socket::socket();
            socket::connect(confd,addr_); 
            inputbuffer.setFd(confd);
            outputbuffer.setFd(confd);
        }
        
        void SingleTcpClient::sendMessage(const char* message,int size)
        {
            inputbuffer.appendInBuffer(message,size);
            inputbuffer.writeInSocket();
        }

        void SingleTcpClient::recvMessage(char* message)
        {
            outputbuffer.readInSocket();
            outputbuffer.getCompleteMessageInBuffer(message);
        }

        void SingleTcpClient::sendFile(const char* path)
        {
            inputbuffer.sendFile(path);
        }

        void SingleTcpClient::recvFile(const char* path)
        {
            outputbuffer.recvFile(path);
        }
    }
}

