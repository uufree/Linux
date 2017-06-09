/*************************************************************************
	> File Name: SingleTcpClient.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月07日 星期日 21时29分43秒
 ************************************************************************/

#include"SingleTcpClient.h"

namespace unet
{
    namespace net
    {   
        SingleTcpClient::SingleTcpClient()
        {};

        SingleTcpClient::~SingleTcpClient()
        {
            ::close(confd);
        };

        void SingleTcpClient::connect(InetAddress* addr_)
        {
            confd = socket::socket();
            socket::connect(confd,addr_); 
            
            int m = 1;
            int n = ::setsockopt(confd,IPPROTO_TCP,TCP_NODELAY,&m,static_cast<socklen_t>(sizeof(int)));
            
            if(n < 0)
            {
                std::cerr << "setsockopt error" << std::endl;
                exit(1);
            }
        }
        
        void SingleTcpClient::sendMessage(const char& message)
        {
            ::send(confd,&message,1,0);
        }

        void SingleTcpClient::recvMessage(char& message)
        {
            ::recv(confd,&message,1,0);
        }
    }
}

