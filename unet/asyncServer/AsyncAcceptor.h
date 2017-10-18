/*************************************************************************
	> File Name: AsyncAcceptor.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月17日 星期一 20时40分14秒
 ************************************************************************/

#ifndef _ASYNCACCEPTOR_H
#define _ASYNCACCEPTOR_H

#include"../Channel.h"

namespace unet
{
    namespace net
    {
        class AsyncAcceptor final
        {
            typedef std::unique_ptr<Channel> ChannelPtr;
            typedef std::function<void(ChannelPtr&&)> InsertChannelCallBack;
            typedef std::function<void(int)> EraseChannelCallBack;

            public:
                explicit AsyncAcceptor(socket::InetAddress& addr_);
                AsyncAcceptor(const AsyncAcceptor& lhs) = delete;
                AsyncAcceptor& operator=(const AsyncAcceptor& lhs) = delete;
                AsyncAcceptor(AsyncAcceptor&& lhs);
                AsyncAcceptor& operator=(AsyncAcceptor&& lhs);
                ~AsyncAcceptor();

                void listen();
                void stopListen();
                bool listened() const
                {return listening;};

                void setInsertChannelCallBack(const InsertChannelCallBack& cb)
                {insertChannelCallBack = cb;};

                void setEraseChannelCallBack(const EraseChannelCallBack& cb)
                {eraseChannelCallBack = cb;};

            private:
                void handleRead();
                 
            private:
                socket::InetAddress& serveraddr;
                socket::Socket listenfd;
                bool listening;

                InsertChannelCallBack insertChannelCallBack;
                EraseChannelCallBack eraseChannelCallBack;
        };
    }
}


#endif

