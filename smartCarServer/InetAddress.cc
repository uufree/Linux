/*************************************************************************
	> File Name: Inettddress.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月17日 星期三 23时45分15秒
 ************************************************************************/

#include"InetAddress.h"

namespace unet
{
    namespace net
    {
        namespace socket
        {
            InetAddress::InetAddress(int port)
            {
                init(static_cast<uint16_t>(port));
            }

            InetAddress::InetAddress(uint16_t port)
            {
                init(port);
            }

            InetAddress::InetAddress(const std::string& ip_,int port) : ip(ip_)
            {
                init(static_cast<uint16_t>(port)); 
            }

            InetAddress::InetAddress(const std::string& ip_,uint16_t port): ip(ip_)
            {
                init(port);
            }

            void InetAddress::init(uint16_t port)
            {
                bzero(&addr,sizeof(addr));
                addr.sin_port = htons(port);
                addr.sin_family = AF_INET;
                addr.sin_addr.s_addr = htonl(INADDR_ANY);
            }

            InetAddress::InetAddress(InetAddress&& lhs) : addr(std::move(lhs.addr)),ip(std::move(lhs.ip))
            {};

            InetAddress& InetAddress::operator=(InetAddress&& lhs)
            {
                if(*this == lhs)
                    return *this;
                else
                {
                    addr = std::move(lhs.addr);
                    ip = std::move(lhs.ip);
                }
                return *this;
            }

            InetAddress::~InetAddress()
            {};
            
            bool operator==(const InetAddress& lhs,const InetAddress& rhs)
            {
                if(lhs.addr.sin_port == rhs.addr.sin_port && lhs.ip == rhs.ip && lhs.addr.sin_port == rhs.addr.sin_port && lhs.addr.sin_addr.s_addr == rhs.addr.sin_addr.s_addr)
                    return true;
                else
                    return false;
            }
        }
    }
}


