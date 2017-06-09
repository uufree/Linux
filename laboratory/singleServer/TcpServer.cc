/*************************************************************************
	> File Name: TcpServer.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月11日 星期六 19时18分41秒
 ************************************************************************/

#include"TcpServer.h"

namespace unet
{
    namespace net
    {
        TcpServer::TcpServer(InetAddress* serveraddr_) : serveraddr(serveraddr_),acceptor(new Acceptor(serveraddr_))
        {
            init();
            std::cout << "服务端已经初始化完毕!" << std::endl;
        };

        void TcpServer::init()
        {   
            acceptor->listen();

            conArray[0] = acceptor->handleRead();
            setNodelay(conArray[0]);
            std::cout << "目标1已经连接！" << std::endl;
            
            conArray[1] = acceptor->handleRead();
            setNodelay(conArray[1]);
            std::cout << "目标2已经连接！" << std::endl;
        }
        
        void TcpServer::handleEvent()
        {
            char ch;
            ::recv(conArray[0],&ch,1,0); 
            std::cout << ch << std::endl;
//            ::send(conArray[1],&ch,1,0);
        }

        void TcpServer::loop()
        {
            while(1)
            {
                handleEvent();
            }
        }
        
        void TcpServer::setNodelay(int fd)
        {
            int m = 1;
            int n = ::setsockopt(fd,IPPROTO_TCP,TCP_NODELAY,&m,static_cast<socklen_t>(sizeof(int)));
            
            if(n < 0)
            {
                std::cerr << "setsockopt error" << std::endl;
                exit(1);
            }
        }

    
    }
}
        


