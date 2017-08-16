/*************************************************************************
	> File Name: TimerEventSystem.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年08月16日 星期三 21时06分58秒
 ************************************************************************/

#include"TimerEventSystem.h"

namespace unet
{
    namespace net
    {
        TimerEventSystem::TimerEventSystem() :
            channelMap(),
            epoller(),
            eventLoop(),
            timerQueue(),
            thread()
        {
            eventLoop.setGetActiveChannelsCallBack(std::bind(&TimerEventSystem::GetActiveChannels,this));
            
            timerQueue.setInsertChannelCallBack(std::bind(&TimerEventSystem::InsertChannel,this,std::placeholders::_1));
            timerQueue.setEraseChannelCallBack(std::bind(&TimerEventSystem::EraseChannel,this,std::placeholders::_1));
            
            thread.setThreadCallBack(std::bind(&TimerEventSystem::ThreadStart,this));
        };

        TimerEventSystem::TimerEventSystem(TimerEventSystem&& lhs) :
            channelMap(std::move(lhs.channelMap)),
            epoller(std::move(lhs.epoller)),
            eventLoop(std::move(lhs.eventLoop)),
            timerQueue(std::move(lhs.timerQueue)),
            thread(std::move(lhs.thread))
        {
            eventLoop.setGetActiveChannelsCallBack(std::bind(&TimerEventSystem::GetActiveChannels,this));
            
            timerQueue.setInsertChannelCallBack(std::bind(&TimerEventSystem::InsertChannel,this,std::placeholders::_1));
            timerQueue.setEraseChannelCallBack(std::bind(&TimerEventSystem::EraseChannel,this,std::placeholders::_1));
            
            thread.setThreadCallBack(std::bind(&TimerEventSystem::ThreadStart,this));
        };

        TimerEventSystem& TimerEventSystem::operator=(TimerEventSystem&& lhs)
        {
            channelMap = std::move(lhs.channelMap);
            epoller = std::move(lhs.epoller);
            eventLoop = std::move(lhs.eventLoop);
            timerQueue = std::move(lhs.timerQueue);
            thread = std::move(lhs.thread);

            return *this;
        };
        
        TimerEventSystem::~TimerEventSystem()
        {};

        void TimerEventSystem::start()
        {
            thread.start();
        }

        void TimerEventSystem::stop()
        {
            timerQueue.stop();
            eventLoop.setQuit();
        }

        void TimerEventSystem::addTimer(time::TimerPtr&& timer)
        {
            timerQueue.addTimer(std::move(timer));
        }

        void TimerEventSystem::InsertChannel(ChannelPtr&& channel)
        {
            channelMap.insert(std::move(channel));
        }

        void TimerEventSystem::EraseChannel(int index)
        {
            channelMap.erase(index);
        }

        void TimerEventSystem::GetActiveChannels()
        {
            epoller.epoll(channelMap);
        }
        
        void TimerEventSystem::ThreadStart()
        {
            timerQueue.start();
            eventLoop.loop();
        }
    }
}
