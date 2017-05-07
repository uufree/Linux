/*************************************************************************
	> File Name: AsyncEventLoop.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月18日 星期二 21时00分10秒
 ************************************************************************/

#ifndef _ASYNCEVENTLOOP_H
#define _ASYNCEVENTLOOP_H

#include"../Epoller.h"

class Channel;

namespace unet
{
    namespace net
    { 
        class AsyncEventLoop final
        {
            typedef std::vector<Channel*> ChannelList;
            typedef std::function<void(ChannelList*)> GetActiveChannelsCallBack;
            typedef std::function<void()> HandleAsyncBuffer;

            public:                                    
                AsyncEventLoop();
                AsyncEventLoop(const AsyncEventLoop&) = delete;
                AsyncEventLoop& operator=(const AsyncEventLoop&) = delete;
                ~AsyncEventLoop() {};
//punlic interface
                void loop();
                void setQuit();
                void setGetActiveChannelsCallBack(const GetActiveChannelsCallBack& cb)
                {activecallback = cb;};//由Acceptor注
                
                ChannelList* getChannelList();
                
                void setHandleAsyncBuffer(const HandleAsyncBuffer& cb)
                {handleasyncbuffer = cb;};

            private:
                bool looping;
                bool quit;
                bool eventhandling;
                ChannelList activechannels;
                GetActiveChannelsCallBack activecallback;
                HandleAsyncBuffer handleasyncbuffer;
        };
    }
}

#endif









