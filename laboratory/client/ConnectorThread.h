/*************************************************************************
	> File Name: ConnectorThread.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月13日 星期四 20时18分08秒
 ************************************************************************/

#ifndef _CONNECTORTHREAD_H
#define _CONNECTORTHREAD_H

#include"Connector.h"
#include"../Thread.h"

namespace unet
{
    namespace net
    {
        class ConnectorThread final
        {
            typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
            typedef std::function<Channel*(int)> ConnectionCallBack;
            typedef std::function<void()> HandleAsyncBuffer;

            public:
                ConnectorThread() : connector(new Connector()),
                thread(new unet::thread::Thread),
                once(0)
                {
                    thread->setThreadCallBack(std::bind(&ConnectorThread::loopStart,this));
                };
                
                ConnectorThread(const ConnectorThread& lhs) = delete;
                ConnectorThread& operator=(const ConnectorThread& lhs) = delete;
                ~ConnectorThread(){};
                
                void setConnectionCallBack(const ConnectionCallBack& cb)
                {connector->setConnectionCallBack(cb);};

                void setHandleAsyncBufferCallBack(const HandleAsyncBuffer& cb)
                {connector->setHandleAsyncBufferCallBack(cb);once++;};

                bool isOnce()
                {return once == 0;};

                void connection(InetAddress* addr)
                {connector->connection(addr);};
/*
                void createChannel()
                {connector->createChannel();};
*/
                void loopStart()
                {connector->start();};

                void start()
                {thread->start();};

            private:
                std::unique_ptr<unet::net::Connector> connector;
                std::unique_ptr<unet::thread::Thread> thread;
                int once;
        };
    }
}


#endif

