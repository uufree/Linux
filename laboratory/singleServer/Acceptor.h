/*************************************************************************
	> File Name: Acceptor.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月10日 星期五 08时45分48秒
 ************************************************************************/

#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include"../Epoller.h"
#include"../EventLoop.h"

namespace unet
{
    namespace net
    {
        class Acceptor final
        {
            public:          
                explicit Acceptor(InetAddress* addr_);
                Acceptor(const Acceptor& lhs) = delete;
                Acceptor& operator=(const Acceptor& lhs) = delete;
                ~Acceptor() {::close(listenfd);};
//public interface
                void listen();
                bool listened() {return listening;};
                int handleRead();
                
            private:
                InetAddress* serveraddr;
                int listenfd;
                bool listening;
        };

    }
}

#endif

