/*************************************************************************
	> File Name: Socket.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月21日 星期二 00时12分57秒
 ************************************************************************/

#include"Socket.h"
#include<iostream>

namespace unet
{
    namespace net
    {
        namespace socket
        {
            int socket(int family,int type,int protocol)
            {
                int n = ::socket(family,type,protocol);
                assert(n > 0);
                return n;
            }

            void setNodelay(int sockfd)
            {
                int m = 1;
                int n = ::setsockopt(sockfd,IPPROTO_TCP,TCP_NODELAY,&m,static_cast<socklen_t>(sizeof(int)));
                assert(n >= 0);
            }

            void setNonBlockAndCloseOnExec(int sockfd)
            {
                int flag = ::fcntl(sockfd,F_GETFL,0);
                flag |= O_NONBLOCK;
                flag |= FD_CLOEXEC;
                int n = ::fcntl(sockfd,F_SETFL,flag);
                assert(n >= 0);
            }

            void listen(int sockfd)
            {
                int n = ::listen(sockfd,COMMSIZE);
                assert(n >= 0);
            }

            int accept(int sockfd)
            {
                int connetfd = ::accept(sockfd,nullptr,nullptr);
                assert(connetfd > 0);
                return connetfd;
            }

            int connect(int sockfd,InetAddress* addr)
            {
                sockaddr_in addr_ = addr->getSockaddr();
                int n = ::connect(sockfd,(sockaddr*)(&addr_),static_cast<socklen_t>(sizeof(struct sockaddr_in)));
                    
                return n;
            }

            void bind(int sockfd,InetAddress* addr)
            {
                sockaddr_in addr_ = addr->getSockaddr();                
                int n = ::bind(sockfd,(sockaddr*)(&addr_),static_cast<socklen_t>(sizeof(struct sockaddr_in)));
                assert(n >= 0);
            }

            void setKeepAlive(int sockfd)
            {
                int m = 1;
                int n = ::setsockopt(sockfd,SOL_SOCKET,SO_KEEPALIVE,&m,static_cast<socklen_t>(sizeof(int)));
                assert(n >= 0);
            }

            void close(int sockfd)
            {
                int n = ::close(sockfd);
                assert(n >= 0);
            }
        }
    }
}
