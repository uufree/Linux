/*************************************************************************
	> File Name: Condition.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月28日 星期二 16时04分46秒
 ************************************************************************/

//封装条件变量，虽然说封装的不咋地，但是还能用

#ifndef _CONDITION_H
#define _CONDITION_H

#include"Mutex.h"

namespace unet
{
    namespace thread
    { 
        class Condition final
        {
            public:
                explicit Condition(MutexLock& mutex_);
            
                Condition(const Condition& lhs) = delete;
                Condition(Condition&& lhs) ;
                Condition& operator=(const Condition& lhs) = delete;
                Condition& operator=(Condition&& lhs);
                ~Condition();

                void notify();
                void notifyAll();
                void wait();
    
            private:
                MutexLock& mutex;
                pthread_cond_t cond;
        };
    }
}

#endif

