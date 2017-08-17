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
            eventMap(),
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
            eventMap(std::move(lhs.eventMap)),
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
            eventMap = std::move(lhs.eventMap);
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
//            ThreadStart();              
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
            channel->setCloseCallBack(std::bind(&TimerEventSystem::EraseChannel,this,std::placeholders::_1));
            channelMap.insert(std::move(channel));
            eventMap.insert(channel->getFd(),channel->getEvent(),epoller.getEpollfd());
        }

        void TimerEventSystem::EraseChannel(int index)
        {
            channelMap.erase(index);
            eventMap.erase(index,epoller.getEpollfd());
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
