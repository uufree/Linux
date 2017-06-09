/*************************************************************************
	> File Name: Acceptor.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月10日 星期五 08时45分54秒
 ************************************************************************/

#include"Acceptor.h"

namespace unet
{
    namespace net
    {
        Acceptor::Acceptor(InetAddress* addr_) :
            serveraddr(addr_),
            listenfd(socket::socket()),
            listening(false)
        {
            ::close(listenfd);
            int fd = socket::socket();
            socket::bind(fd,serveraddr);//设置监听套接字 
            listenfd = fd;

            int m = 1;
            int n = ::setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&m,static_cast<socklen_t>(sizeof(int)));
                
            if(n == -1)
            {
                std::cerr << "setsockopt error!" << std::endl;
                exit(1);
            }
        }

        void Acceptor::listen()
        {//将listenfd设置为可读的，并且加入到epoller中
            listening = true;
            socket::listen(listenfd);
        }
        
        int Acceptor::handleRead()
        {//监听到的时间的处理方法
            int confd = socket::accept(listenfd);
            assert(confd >= 0);

            return confd;
        }

    }
}

