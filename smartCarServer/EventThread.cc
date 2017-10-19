/*************************************************************************
	> File Name: EventThread.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年10月19日 星期四 22时21分40秒
 ************************************************************************/

#include"EventThread.h"

namespace unet
{
    namespace net
    {
        EventThread::EventThread() :
            channelMap(),
            eventMap(),
            epoller(),
            eventLoop(),
            taskQueue(),
            thread(),
            started(false)
        {
            eventLoop.setGetActiveChannelsCallBack(std::bind(&EventThread::GetActiveChannels,this));
            thread.setThreadCallBack(std::bind(&EventThread::ThreadStart,this));
            thread.start();
        }

        EventThread::EventThread(EventThread&& lhs) :
            channelMap(std::move(lhs.channelMap)),
            eventMap(std::move(lhs.eventMap)),
            epoller(std::move(lhs.epoller)),
            eventLoop(std::move(lhs.eventLoop)),
            taskQueue(std::move(lhs.taskQueue)),
            thread(std::move(lhs.thread)),
            started(false)
        {};
        
        EventThread& EventThread::operator=(EventThread&& lhs)
        {
            if(lhs.isStart())
                lhs.stop();

            channelMap = std::move(lhs.channelMap);
            eventMap = std::move(lhs.eventMap);
            epoller = std::move(lhs.epoller);
            eventLoop = std::move(lhs.eventLoop);
            taskQueue = std::move(lhs.taskQueue);
            thread = std::move(lhs.thread);

            started = false;

            return *this;
        }

        EventThread::~EventThread()
        {
            if(started)
            {
                eventLoop.setQuit();
                channelMap.clear();
                eventMap.clear();
                thread.join();
            }
        }
        
    }
}
