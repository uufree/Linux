/*************************************************************************
	> File Name: TcpConnectionMap.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月12日 星期一 22时34分35秒
 ************************************************************************/

#include"TcpConnectionMap.h"

namespace unet
{
    namespace net
    {
        TcpConnectionMap::TcpConnectionMap()
        {};

        TcpConnectionMap::TcpConnectionMap(TcpConnectionMap&& lhs) :
            tcpConnectionMap(std::move(lhs.tcpConnectionMap))
        {};

        TcpConnectionMap& TcpConnectionMap::operator=(TcpConnectionMap&& lhs)
        {
            tcpConnectionMap = std::move(lhs.tcpConnectionMap);
            return *this;
        }

        TcpConnectionMap::~TcpConnectionMap()
        {};

        void TcpConnectionMap::insert(int fd)
        {
            thread::MutexLockGuard guard(mutex);
            TcpConnectionPtr tcp(new TcpConnection(fd));
            tcpConnectionMap.insert({fd,tcp});
        }

        void TcpConnectionMap::insert(const TcpConnectionPtr& lhs)
            {
                thread::MutexLockGuard guard(mutex);
                tcpConnectionMap.insert({lhs->getFd(),lhs});
        }

        void TcpConnectionMap::erase(int fd) 
        {
            thread::MutexLockGuard guard(mutex);
            tcpConnectionMap.erase(fd);
        }
    }
}

