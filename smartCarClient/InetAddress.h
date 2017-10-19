/*************************************************************************
	> File Name: InetAddress.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月27日 星期一 20时58分25秒
 ************************************************************************/

//封装IP地址

#ifndef _INETADDRESS_H
#define _INETADDRESS_H

#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<string>

namespace unet
{
    namespace net
    {
        namespace socket
        {
            class InetAddress final
            {   
                static const int IPV4SIZE = 16;
                static const int IPV6SIZE = 32;
                friend bool operator==(const InetAddress& lhs,const InetAddress& rhs);

                public:
                    explicit InetAddress(int port);            
                    explicit InetAddress(uint16_t port);
                    explicit InetAddress(const std::string& ip,int port);
                    explicit InetAddress(const std::string& ip,uint16_t port);

                    InetAddress(const InetAddress& lhs) = delete; 
                    InetAddress(InetAddress&& lhs);
                    InetAddress& operator=(const InetAddress& lhs) = delete;
                    InetAddress& operator=(InetAddress&& lhs);
                    ~InetAddress();
                    

                    const std::string& getIpString() const
                    {return ip;};
                    
                    const sockaddr_in& getSockaddr() const
                    {return addr;}
            
                    uint32_t getIp() const
                    {
                        in_addr_t addr_ = ntohl(addr.sin_addr.s_addr);
                        return static_cast<uint32_t>(addr_);
                    };   

                    uint16_t getPort() 
                    {
                        in_port_t port = ntohs(addr.sin_port);
                        return static_cast<uint16_t>(port);
                    };

                private:
                    void init(uint16_t port);
                
                private:
                    struct sockaddr_in addr;
                    std::string ip = "INADDR_ANY";
            };

            bool operator==(const InetAddress& lhs,const InetAddress& rhs);
        
        }
    }
}

#endif
