/*************************************************************************
	> File Name: ChannelMap.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月12日 星期一 22时34分47秒
 ************************************************************************/

/*设计理念：借用粗粒度锁同步数据源,交由上层统一管理
 
 *1.核心目的：保证多线程数据安全
 *2.整个程序中仅存一份数据源，所有的处理通过都一致的接口调用
 */


#ifndef _CHANNELMAP_H
#define _CHANNELMAP_H

#include"Channel.h"
#include"Mutex.h"

#include<set>
#include<map>
#include<memory>

namespace unet
{
    namespace net
    {
        typedef std::shared_ptr<Channel> ChannelPtr;
        
        class ChannelMap final
        {
            public:
                explicit ChannelMap();
                ChannelMap(const ChannelMap& lhs) = delete;
                ChannelMap(ChannelMap&& lhs);
                ChannelMap& operator=(const ChannelMap& lhs) = delete;
                ChannelMap& operator=(ChannelMap&& lhs);
                ~ChannelMap();

                void swap(ChannelMap& lhs) = delete;
            
                int size() const
                {return channelMap.size();};

                bool empty() const
                {return channelMap.empty();};
                
                ChannelPtr findChannel(int fd)
                {return channelMap[fd];};

                void insert(const ChannelPtr& channel);
                void insert(int fd,ChannelType type);
                void erase(int fd);
                

            private:
                thread::MutexLock mutex;
                std::map<int,ChannelPtr> channelMap;
        };
    }
}


#endif
