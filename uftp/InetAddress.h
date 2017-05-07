/*************************************************************************
	> File Name: InetAddress.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月27日 星期一 20时58分25秒
 ************************************************************************/

#ifndef _INETADDRESS_H
#define _INETADDRESS_H

#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<string>
#include<iostream>

namespace unet
{
    namespace net
    {
        class InetAddress final
        {   
            public:
                static const int IPV4SIZE = 16;
                static const int IPV6SIZE = 32;
                
                InetAddress(const InetAddress& addr) = delete;
                InetAddress& operator=(const InetAddress& addr_) = delete;
                ~InetAddress() {};

                explicit InetAddress(uint16_t port)
                {
                    bzero(&addr,sizeof(addr));
                    addr.sin_port = htons(port);
                    addr.sin_family = AF_INET;
                    addr.sin_addr.s_addr = htonl(INADDR_ANY);
                };

                explicit InetAddress(const std::string& ip,uint16_t port) : ip_(ip)
                {  
                    bzero(&addr,sizeof(addr));
                    addr.sin_port = htons(port);
                    addr.sin_family = AF_INET;
                    inet_pton(AF_INET,ip.c_str(),&addr.sin_addr.s_addr);
//                    addr.sin_addr.s_addr = htonl(addr.sin_addr.s_addr);
                };   

                std::string getIpString()
                {
                    return ip_;
                };
            
                uint32_t getIp() 
                {
                    in_addr_t addr_ = ntohl(addr.sin_addr.s_addr);
                    return static_cast<uint32_t>(addr_);
                };   

                uint16_t getPort() 
                {

                    in_port_t port = ntohs(addr.sin_port);
                    return static_cast<uint16_t>(port);
                };

                sockaddr_in getSockaddr() const
                {
                    return addr;
                }

            private:
                struct sockaddr_in addr;
                std::string ip_ = "INADDR_ANY";//网络字节序的准换有问题，所以只能这么写
        };
    }
}

#endif
