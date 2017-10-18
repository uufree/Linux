/*************************************************************************
	> File Name: Socket.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月21日 星期二 00时12分57秒
 ************************************************************************/

#include"Socket.h"
#include<string.h>

namespace unet
{
    namespace net
    {
        namespace socket
        {
            Socket::Socket(SocketType type_) : type(type_),
                socketfd(socket()),
                bit(0x00)
            {};

            Socket::Socket(int connectfd) : type(CONNECT),
                socketfd(connectfd),
                bit(0x00)
            {};

            Socket::Socket(const Socket& lhs) : type(lhs.type),
                socketfd(lhs.socketfd),
                bit(lhs.bit)
            {};

            Socket::Socket(Socket&& lhs) : type(std::move(lhs.type)),
                socketfd(std::move(lhs.socketfd)),
                bit(std::move(lhs.bit))
            {};

            Socket& Socket::operator=(const Socket& lhs)
            {
                if(lhs == *this)
                    return *this;
                else
                {
                    close(socketfd);
                    this->socketfd = lhs.socketfd;
                    this->bit = lhs.bit;
                }
                return *this;
            }

            Socket& Socket::operator=(Socket&& lhs)
            {
                if(lhs == *this)
                    return *this;
                else
                {
                    close(socketfd);
                    this->socketfd = std::move(lhs.socketfd);
                    this->bit = std::move(lhs.bit);
                }
                return *this;
            }
            
            Socket::~Socket()
            {
                unet::net::socket::close(socketfd);
            }
            
            int socket(int family,int type,int protocol)
            {
                int n = ::socket(family,type,protocol);
                if(n  == -1)
                    unet::handleError(errno);
                return n;
            }

            void listen(int socketfd)
            {
                int n = ::listen(socketfd,COMMSIZE);
                if(n == -1)
                    unet::handleError(errno);
            }

            int accept(int socketfd)
            {
                int connectfd = ::accept(socketfd,nullptr,nullptr);
                if(connectfd < 0)
                    unet::handleError(errno);
                return connectfd;
            }
            
            void connect(int socketfd,InetAddress& addr)
            {
                sockaddr_in addr_ = addr.getSockaddr();
                int n = ::connect(socketfd,(sockaddr*)(&addr_),static_cast<socklen_t>(sizeof(struct sockaddr_in)));
                
                if(n == -1)
                    unet::handleError(errno);
            }
            
            void bind(int sockfd,InetAddress& addr)
            {
                sockaddr_in addr_ = addr.getSockaddr();                
                int n = ::bind(sockfd,(sockaddr*)(&addr_),static_cast<socklen_t>(sizeof(struct sockaddr_in)));
                
                if(n < 0)
                    unet::handleError(errno);
            }
            
            void close(int sockfd)
            {
                int n = ::close(sockfd);
                
                if(n < 0)
                    unet::handleError(errno);
            }

            bool setKeepAlive(int socketfd)
            {
                int m = 1;                
                int n = ::setsockopt(socketfd,SOL_SOCKET,SO_KEEPALIVE,&m,static_cast<socklen_t>(sizeof(int)));
                
                if(n == -1)
                    unet::handleError(errno);
                return true;
            }

            bool setNodelay(int socketfd)
            {
                int m = 1;
                int n = ::setsockopt(socketfd,IPPROTO_TCP,TCP_NODELAY,&m,static_cast<socklen_t>(sizeof(int)));
                
                if(n == -1)
                    unet::handleError(errno);
                return true;
            }

            bool setNonBlockAndCloseOnExec(int socketfd)
            {
                int flag = ::fcntl(socketfd,F_GETFL,0);
                flag |= O_NONBLOCK;
                flag |= FD_CLOEXEC;
                int n = ::fcntl(socketfd,F_SETFL,flag);

                if(n == -1)
                    unet::handleError(errno);
                return true;
            }
            
            bool setReuseAddr(int socketfd)
            {
                int m = 1;
                int n = ::setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,&m,static_cast<socklen_t>(sizeof(int)));
                
                if(n == -1)
                    unet::handleError(errno);
                return true;
            }

            bool setReusePort(int socketfd)
            {
                int m = 1;
                int n = ::setsockopt(socketfd,SOL_SOCKET,SO_REUSEPORT,&m,static_cast<socklen_t>(sizeof(int)));
                
                if(n == -1)
                    unet::handleError(errno);
                return true;
            }
            
            void setKeepAlive(Socket& sock)
            {
                setKeepAlive(sock.getFd());
                sock.setKeepAliveBit();
            }

            void setNodelay(Socket& sock)
            {
                setNodelay(sock.getFd());
                sock.setNoDelayBit();
            }

            void setNonBlockAndCloseOnExec(Socket& sock)
            {
                setNonBlockAndCloseOnExec(sock.getFd());
                sock.setNonBlockAndCloseOnExecBit();
            }

            void setReuseAddr(Socket& sock)
            {
                setReuseAddr(sock.getFd());
                sock.setReuseAddrBit();
            }

            void setReusePort(Socket& sock)
            {
                setReusePort(sock.getFd());
                sock.setReusePortBit();
            }
            
            void listen(Socket& lhs)
            {
                listen(lhs.getFd());
            }

            void connect(Socket& lhs,InetAddress& addr)
            {
                connect(lhs.getFd(),addr);
            }

            void bind(Socket& lhs,InetAddress& addr)
            {
                bind(lhs.getFd(),addr);
            }

            int accept(Socket& lhs)
            {
                return accept(lhs.getFd());
            }

            bool operator==(const Socket& lhs,const Socket& rhs)
            {
                if(rhs.socketfd == lhs.socketfd && rhs.bit == lhs.bit && rhs.type == lhs.type)
                    return true;
                else
                    return false;
            }
        }
    }
}
