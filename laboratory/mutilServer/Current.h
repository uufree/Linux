/*************************************************************************
	> File Name: Current.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月28日 星期二 16时38分36秒
 ************************************************************************/

#ifndef _CURRENT_H
#define _CURRENT_H

#include"../Epoller.h"
#include"../EventLoop.h"

namespace unet
{
    namespace thread
    {
        //这个类是为了将Epoller和EventLoop聚合起来，形成的中间类
        class Current final
        {
            typedef std::vector<unet::net::Channel*> ChannelList;
            public:
                explicit Current();
                Current(const Current& lhs) = delete;
                Current& operator=(const Current& lhs) = delete;
                ~Current(){};
                
                void startLoop()
                {loop->loop();};

                unet::net::Epoller* getEpoller()
                {return epoller.get();};

                int getConSize()
                {return epoller->getConSize();};

                void getActiveChannels(ChannelList* channels);
                
                void addInEpoller(unet::net::Channel* channel_);

                void quit()
                {loop->setQuit();};

            private:
                std::unique_ptr<unet::net::Epoller> epoller;
                std::unique_ptr<unet::net::EventLoop> loop;
        };
    }
}






















#endif

