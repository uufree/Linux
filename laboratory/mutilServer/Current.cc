/*************************************************************************
	> File Name: Current.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月28日 星期二 16时38分43秒
 ************************************************************************/

#include"Current.h"

namespace unet
{
    namespace thread
    {
        Current::Current() : epoller(new unet::net::Epoller),loop(new unet::net::EventLoop)
        {
            loop->setGetActiveChannelsCallBack(std::bind(&Current::getActiveChannels,this,std::placeholders::_1));
        }

        void Current::getActiveChannels(ChannelList* channels)
        {
            epoller->epoll(channels);
        };

        void Current::addInEpoller(unet::net::Channel* channel_)
        {
            epoller->addInChannelMap(channel_);
        };
    }
}

