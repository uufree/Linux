/*************************************************************************
	> File Name: Socket.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月27日 星期一 23时56分55秒
 ************************************************************************/

#ifndef _SOCKET_H
#define _SOCKET_H

#include<assert.h>
#include"InetAddress.h"
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<assert.h>
#include<netinet/tcp.h>
#include<fcntl.h>
#include<unistd.h>

namespace unet
{
    namespace net
    {
        namespace socket
        {
            const static int COMMSIZE = 256;

            int socket(int family=AF_INET,int type=SOCK_STREAM,int protocol=IPPROTO_TCP);
            
            void setNodelay(int sockfd);

            void setNonBlockAndCloseOnExec(int sockfd);

            void listen(int sockfd);

            int accept(int sockfd);

            int connect(int sockfd,InetAddress* addr);

            void bind(int sockfd,InetAddress* addr);

            void setKeepAlive(int sockfd);

            void close(int sockfd);

            class Socket final
            {//用RAII处理sockfd
                public:
                    explicit Socket(int sockfd_) : sockfd(sockfd_)
                    {};
                
                    Socket(const Socket&) = delete;
                    Socket(Socket&&) = delete;

                    ~Socket()
                    {
                        assert(::close(sockfd) == 0);
                    }

                    int getFd() const
                    {   
                        return sockfd;
                    }

                    void setFd(int fd_)
                    {sockfd = fd_;};

                private:
                    int sockfd;
            };
            
        }
    }
}



#endif

