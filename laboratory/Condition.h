/*************************************************************************
	> File Name: Condition.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月28日 星期二 16时04分46秒
 ************************************************************************/

#ifndef _CONDITION_H
#define _CONDITION_H

#include<pthread.h>
#include"Mutex.h"

namespace unet
{
    namespace thread
    { 
        class Condition final
        {
            public:
                explicit Condition(MutexLock& mutex_) : mutex(mutex_)
                {
                    pthread_cond_init(&cond,NULL);
                }

                ~Condition()
                {
                    pthread_cond_destroy(&cond);
                }

                void notify()
                {
                    pthread_cond_signal(&cond);
                }

                void notifyAll()
                {
                    pthread_cond_broadcast(&cond);
                }

                void wait()
                {
//                    assert(mutex.getPid() != 0);//mutex is locked
                    pthread_cond_wait(&cond,mutex.getMutex());    
                }

            private:
                MutexLock& mutex;
                pthread_cond_t cond;
        };
    }
}




















#endif

