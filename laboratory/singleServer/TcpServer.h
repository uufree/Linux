/*************************************************************************
  > File Name: TcpServer.h
  > Author: uuchen
  > Mail: 1319081676@qq.com
  > Created Time: 2017年03月11日 星期六 19时18分24秒
 ************************************************************************/

#ifndef _TCPSERVER_H
#define _TCPSERVER_H

#include<assert.h>
#include<algorithm>
#include"Acceptor.h"
#include<array>

namespace unet
{
    namespace net
    {
        class TcpServer final
        {

            public:     
                explicit TcpServer(InetAddress* serveraddr_);
                TcpServer(const TcpServer& lhs) = delete;
                TcpServer& operator=(const TcpServer& lhs) = delete;
                ~TcpServer() {};
               
                void loop();
                void handleEvent();

            private:
                void init();
                void setNodelay(int fd);

            private:
                InetAddress* serveraddr;
                std::unique_ptr<Acceptor> acceptor;
                std::array<int,2> conArray;
        };

    }
}






































#endif

