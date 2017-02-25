/*************************************************************************
	> File Name: Condition.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月26日 星期日 00时06分47秒
 ************************************************************************/

#ifndef _CONDITION_H
#define _CONDITION_H

#include<pthread.h>
#include"Mutex.h"

namespace Tool
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
                pthread_cond_destory(&cond);
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
                assert(mutex.getPid() != 0);//mutex is locked
                pthread_cond_wait(&cond,mutex.getMutex());    
            }

        private:
            MutexLock& mutex;
            pthread_cond_t cond;

    };

}
#endif

