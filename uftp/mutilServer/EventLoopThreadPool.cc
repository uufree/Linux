/*************************************************************************
	> File Name: EventLoopThreadPool.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月28日 星期二 16时56分01秒
 ************************************************************************/

#include"EventLoopThreadPool.h"

namespace unet
{
    namespace thread
    {
        EventLoopThreadPool::EventLoopThreadPool(int size) :
            threadsize(size)
        {
            threads = new EventLoopThread[threadsize];
        }

        EventLoopThreadPool::~EventLoopThreadPool()
        {
            delete [] threads; 
        }

        void EventLoopThreadPool::start()
        {
            for(int i=0;i<threadsize;++i)
            {
                threadlist.push_back(threads[i].getThreadId());
                epollerlist.push_back(threads[i].getEpoller());
                threads[i].start();
            }
            iter = epollerlist.begin();
        }

        void EventLoopThreadPool::joinAll()
        {
            for(auto iter=threadlist.begin();iter!=threadlist.end();++iter)
                ::pthread_join(*iter,NULL);
        }

        void EventLoopThreadPool::addInChannelMap(unet::net::Channel* channel)
        {
            for(auto it=epollerlist.begin();it!=epollerlist.end();++it)              (*it)->getInfo();
            
            (*iter)->addInChannelMap(channel);
            if(iter != epollerlist.end())
                ++iter;
            
            for(auto it=epollerlist.begin();it!=epollerlist.end();++it)              (*it)->getInfo();
            
        }
    }
}
            





