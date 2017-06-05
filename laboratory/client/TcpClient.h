/*************************************************************************
  > File Name: TcpClient.h
  > Author: uuchen
  > Mail: 1319081676@qq.com
  > Created Time: 2017年03月21日 星期二 20时32分11秒
 ************************************************************************/

#ifndef _TCPCLIENT_H
#define _TCPCLIENT_H

#include"ConnectorThread.h"

namespace unet
{
    namespace net
    {
        class TcpClient final
        {
            typedef std::function<void(Buffer*,Buffer*)> MessageCallBack;
            typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
            typedef std::map<int,TcpConnectionPtr> TcpConnectionPtrMap;
            
            public:
                explicit TcpClient();
                TcpClient(const TcpClient& lhs) = delete;
                TcpClient& operator=(const TcpClient& lhs) = delete;
                ~TcpClient() {};

                void setReadCallBack(const MessageCallBack& cb)
                {readcallback = cb;};

                void setWriteCallBack(const MessageCallBack& cb)
                {writecallback = cb;};
            
                void setDrivedCallBack(const MessageCallBack& cb)
                {drivedcallback = cb;};
                
                void handleDiedTcpConnection(int fd_);
                Channel* newConnectionCallBack(int fd_);

                void start()
                {connector->start();};
                
                void connection(InetAddress* addr)
                {connector->connection(addr);};
                
                void closeConnection()
                {
                    if(tcpconnectionptrmap.size() == 1)
                    {
                        int fd = tcpconnectionptrmap.begin()->first;
                        handleDiedTcpConnection(fd);
                    }
                }

                void writeInAsyncBuffer(const std::string str)
                {
                    unet::thread::MutexLockGuard guard(lock);
                    asyncbuffer.push_back(str);
                }

            private:

                void handleAsyncBuffer(Buffer* outputbuffer)
                {
                    std::vector<std::string> vec;
                    {
                        unet::thread::MutexLockGuard guard(lock);
                        if(asyncbuffer.size() != 0)
                            asyncbuffer.swap(vec);    
                    }
                
                    for(auto iter=vec.begin();iter!=vec.end();++iter)
                        outputbuffer->appendInBuffer(iter->c_str());
                    
                    if(outputbuffer->getDataSize() > 0)
                    {
                        std::cout << "将要发送～" << std::endl; 
                        outputbuffer->writeInSocket();
                        std::cout << "发送完毕～" << std::endl; 
                    }
                }

                std::unique_ptr<ConnectorThread> connector;
                MessageCallBack readcallback,writecallback,drivedcallback;
                std::vector<std::string> asyncbuffer;
                unet::thread::MutexLock lock;
                TcpConnectionPtrMap tcpconnectionptrmap;
        };
    }
}
















































#endif

