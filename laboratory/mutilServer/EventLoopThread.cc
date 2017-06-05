/*************************************************************************
	> File Name: EventLoopThread.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月28日 星期二 17时07分46秒
 ************************************************************************/

#include"EventLoopThread.h"

namespace unet
{
    namespace thread
    {
        EventLoopThread::EventLoopThread() : current(new Current),thread(new Thread)
        {
            thread->setThreadCallBack(std::bind(&EventLoopThread::loop,this,current.get()));
        };

        EventLoopThread::EventLoopThread(const EventLoopThread& lhs) :
            current(lhs.current.get()),
            thread(lhs.thread.get())
        {};

        EventLoopThread::EventLoopThread(EventLoopThread&& lhs) : 
            current(std::move(lhs.current)),
            thread(std::move(lhs.thread))
        {};

        EventLoopThread& EventLoopThread::operator=(const EventLoopThread& lhs)
        {
            (this->current).reset(lhs.current.get());
            (this->thread).reset(lhs.thread.get());
            return *this;
        }

        EventLoopThread& EventLoopThread::operator=(EventLoopThread&& lhs)
        {
            (this->current).reset(lhs.current.get());
            (this->thread).reset(lhs.thread.get());
            return *this;
        }

        void EventLoopThread::loop(Current* current_)
        {
            current_->startLoop();
        }
        
        void EventLoopThread::setThreadCallBack()
        {
            thread->setThreadCallBack(std::bind(&EventLoopThread::loop,this,current.get()));
        };
    }
}
