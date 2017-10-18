 /*************************************************************************
	> File Name: TcpConnectionMap.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月12日 星期一 22时34分26秒
 ************************************************************************/

//由Server统一管理的数据集,表连接

#ifndef _TCPCONNECTIONMAP_H
#define _TCPCONNECTIONMAP_H

#include"Mutex.h"
#include"TcpConnection.h"

#include<map>
#include<memory>

namespace unet
{
    namespace net
    {
        typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
        
        class TcpConnectionMap final
        {
            public:
                TcpConnectionMap();
                TcpConnectionMap(const TcpConnectionMap&) = delete;
                TcpConnectionMap(TcpConnectionMap&& lhs);
                TcpConnectionMap& operator=(const TcpConnectionMap&) = delete;
                TcpConnectionMap& operator=(TcpConnectionMap&& lhs);
                ~TcpConnectionMap();

                void swap(TcpConnectionMap& lhs) = delete;

                int size() const
                {return tcpConnectionMap.size();};
                
                bool empty() const
                {return tcpConnectionMap.empty();};

                void insert(int fd);
                void insert(const TcpConnectionPtr& lhs);
                void erase(int fd);
                
                TcpConnectionPtr& find(int fd)
                {return tcpConnectionMap[fd];};

            private:
                thread::MutexLock mutex;
                std::map<int,TcpConnectionPtr> tcpConnectionMap;
        };
    }
}



#endif
