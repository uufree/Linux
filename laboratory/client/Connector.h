/*************************************************************************
	> File Name: Connector.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月21日 星期二 20时36分08秒
 ************************************************************************/

#ifndef _CONNECTOR_H
#define _CONNECTOR_H

#include"../Epoller.h"
#include"AsyncEventLoop.h"

namespace unet
{
    namespace net
    {
        class Connector final
        {
            typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
            typedef std::function<Channel*(int)> ConnectionCallBack;
            typedef std::vector<Channel*> ChannelList;
            typedef std::function<void()> HandleAsyncBuffer;

            public:
                Connector();
                Connector(const Connector& lhs) = delete;
                Connector& operator=(const Connector& lhs) = delete;
                ~Connector(){};
                
                void setConnectionCallBack(const ConnectionCallBack& cb)
                {connectioncallback = cb;};
                
                void setHandleAsyncBufferCallBack(const HandleAsyncBuffer& cb)
                {loop->setHandleAsyncBuffer(cb);};

//                int createConnection(InetAddress* addr_);
                void connection(InetAddress* addr_);
                void start();
                void getActiveChannels(ChannelList* channels);
                
            private:
                ConnectionCallBack connectioncallback;
                std::unique_ptr<Epoller> epoller;
                std::unique_ptr<AsyncEventLoop> loop;
        };
    }
}
            





        





















#endif

