 /************************************************************************
	> File Name: AsyncTcpServer.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月17日 星期一 16时27分51秒
 ************************************************************************/

#ifndef _ASYNCTCPSERVER_H
#define _ASYNCTCPSERVER_H

#include"AsyncAcceptor.h"
#include"../TcpConnectionMap.h"
#include"../ChannelMap.h"
#include"../EventMap.h"
#include"../TaskPool.h"
#include"../Epoller.h"
#include"../EventLoop.h"
#include"../TimerQueue.h"

namespace unet
{
    namespace net
    {
        class AsyncTcpServer final
        {
            typedef std::function<void(Buffer*,Buffer*)> MessageCallBack;
            typedef std::vector<ChannelPtr> ChannelList;

            public:
                explicit AsyncTcpServer(socket::InetAddress& server,int size = 2);
                AsyncTcpServer(const AsyncTcpServer& lhs) = delete;
                AsyncTcpServer& operator=(const AsyncTcpServer& lhs) = delete;
                AsyncTcpServer(AsyncTcpServer&& lhs);
                AsyncTcpServer& operator=(AsyncTcpServer&& lhs);
                ~AsyncTcpServer();

                void setReadCallBack(const MessageCallBack& cb)
                {readCallBack = cb;};
                
                void start();
                void addTimer(time::TimerPtr&& timer);

            private:
                void InsertChannel(ChannelPtr&& channel);
                void EraseChannel(int fd);
                void GetActiveChannels();

            private:
                socket::InetAddress& serveraddr;

                TcpConnectionMap tcpconnectionMap;
                ChannelMap channelMap;
                EventMap eventMap;
                ChannelList channelList;
                thread::TaskPool pool;                

                Epoller epoller;
                EventLoop eventLoop;
                AsyncAcceptor asyncAcceptor;
                time::TimerQueue timerQueue;

                MessageCallBack readCallBack;
        };
    }
}

#endif

