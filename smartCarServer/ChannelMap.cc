/*************************************************************************
	> File Name: ChannelMap.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月12日 星期一 22时34分54秒
 ************************************************************************/

#include"ChannelMap.h"

namespace unet
{
    namespace net
    {
        ChannelMap::ChannelMap()
        {};

        ChannelMap::ChannelMap(ChannelMap&& lhs) : 
            channelMap(std::move(lhs.channelMap))
        {};

        ChannelMap& ChannelMap::operator=(ChannelMap&& lhs)
        {
            channelMap = std::move(lhs.channelMap);

            return *this;
        }
        
        ChannelMap::~ChannelMap()
        {};

        void ChannelMap::insert(int fd,ChannelType type)
        {
            thread::MutexLockGuard guard(mutex);
            net::ChannelPtr channel_(new Channel(fd,type));
            channelMap.insert({fd,channel_});
        }

        void ChannelMap::insert(const ChannelPtr& channel)
        {
            thread::MutexLockGuard gurad(mutex);
            channelMap.insert({channel->getFd(),channel});
        }

        void ChannelMap::erase(int fd)
        {
            thread::MutexLockGuard guard(mutex);
            channelMap.erase(fd);
        }

        void ChannelMap::clear()
        {
            thread::MutexLockGuard guard(mutex);
            channelMap.clear();
        }
    }
}

