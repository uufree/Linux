/*************************************************************************
	> File Name: Condition.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月17日 星期一 19时35分29秒
 ************************************************************************/

#include"Condition.h"
#include<utility>

namespace unet
{
    namespace thread
    {
        Condition::Condition(MutexLock& mutex_) :
            mutex(mutex_)
        {
            if(pthread_cond_init(&cond,NULL) < 0)
                unet::handleError(errno);
        }

        Condition::Condition(Condition&& lhs) :
            mutex(lhs.mutex),
            cond(std::move(lhs.cond))
        {};
        
        Condition& Condition::operator=(Condition&& lhs)
        {
            mutex = std::move(lhs.mutex);
            cond = std::move(lhs.cond);
            
            return *this;
        }
        
        Condition::~Condition()
        {
            if(pthread_cond_destroy(&cond) < 0)
                unet::handleError(errno);
        }
        
        void Condition::notify()
        {
            if(pthread_cond_signal(&cond) < 0)
                unet::handleError(errno);
        }

        void Condition::notifyAll()
        {
            if(pthread_cond_broadcast(&cond) < 0)
                unet::handleError(errno);
        }

        void Condition::wait()
        {
            if(pthread_cond_wait(&cond,&mutex.getMutex()) < 0)
                unet::handleError(errno);
        }
        

        
    }
}

