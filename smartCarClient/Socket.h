/*************************************************************************
	> File Name: Socket.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月27日 星期一 23时56分55秒
 ************************************************************************/

/*      封装套接字及其属性
 *在Socket类中用8位来标志socket属性，最高的两位保留，从低到高依次为：
 * used
 * keepalive
 * nodelay
 * noblock and closeonexec 
 * reuseaddr
 * reuseport
 */

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
#include"error.h"

namespace unet
{
    namespace net
    {
        namespace socket
        {
            enum SocketType{CONNECT,LISTEN};
            const static int COMMSIZE = 256;
            
            class Socket final
            {//用RAII处理sockfd
                friend bool operator==(const Socket& lhs,const Socket& rhs);
                
                public:
                    explicit Socket(SocketType type_);
                    explicit Socket(int connectfd);

                    explicit Socket(const Socket& socketfd_);
                    explicit Socket(Socket&& socketfd);
                    Socket& operator=(const Socket& lhs);
                    Socket& operator=(Socket&& lhs);
                    ~Socket();
                    
                    int getFd() const
                    {  
                        assert(socketfd >= 0);
                        return socketfd;
                    }

                    SocketType getType() const
                    {return type;}

                    bool isUsed() const
                    {return bit & (1 << 5);};
                    void setUsedBit()
                    {bit |= (1 << 5);}

                    bool isKeepAlive() const
                    {return bit & (1 << 4);};
                    void setKeepAliveBit()
                    {bit |= (1 << 4);}

                    bool isNoDelay() const
                    {return bit & (1 << 3);};
                    void setNoDelayBit()
                    {bit |= (1 << 3);}

                    bool isNonBlockAndCloseOnExec() const
                    {return bit & (1 << 2);};
                    void setNonBlockAndCloseOnExecBit()
                    {bit |= (1 << 2);}
                    
                    bool isReuseAddr() const
                    {return bit & (1 << 1);};
                    void setReuseAddrBit()
                    {bit |= (1 << 1);}
                    
                    bool isReusePort() const
                    {return bit & (1 << 0);};
                    
                    void setReusePortBit()
                    {bit |= (1 << 0);}
                
                private:
                    const SocketType type;
                    mutable int socketfd;
                    unsigned char bit;
            };

            bool operator==(const Socket& lhs,const Socket& rhs);
            
            int socket(int family=AF_INET,int type=SOCK_STREAM,int protocol=IPPROTO_TCP);
            void listen(int socketfd);
            int accept(int socketfd);
            void connect(int socketfd,InetAddress& addr);
            void bind(int socketfd,InetAddress& addr);
            void close(int socketfd);
//这五组对fd的操作不暴露给用户        
            bool setKeepAlive(int socketfd);
            bool setNodelay(int sockfd);
            bool setNonBlockAndCloseOnExec(int socketfd);
            bool setReuseAddr(int socketfd);
            bool setReusePort(int socketfd);

            void setKeepAlive(Socket& sock);
            void setNodelay(Socket& sock);
            void setNonBlockAndCloseOnExec(Socket& sock);
            void setReuseAddr(Socket& sock);
            void setReusePort(Socket& sock);
            
            void listen(Socket& lhs);
            void connect(Socket& lhs,InetAddress& addr);
            void bind(Socket& lhs,InetAddress& addr);
            int accept(Socket& lhs);
        }
    }
}



#endif

