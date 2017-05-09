/*************************************************************************
	> File Name: SingleTcpClient.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月07日 星期日 21时29分29秒
 ************************************************************************/

#ifndef _SINGLETCPCLIENT_H
#define _SINGLETCPCLIENT_H

#include"../Epoller.h"

namespace unet
{
    namespace net
    {
        typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

        class SingleTcpClient final
        {
            public:
                SingleTcpClient();
                SingleTcpClient(const SingleTcpClient& lhs) = delete;
                SingleTcpClient& operator=(const SingleTcpClient& lsh);
                ~SingleTcpClient();

                void connect(InetAddress* addr_);
                void sendMessage(const char* message);
                void recvMessage(char* message);
                void sendFile(const char* path);
                void recvFile(const char* path);
                int getConfd() const
                {return confd;};

            private:
                int confd;
                Buffer inputbuffer;
                Buffer outputbuffer;
        };
    }
}



#endif
