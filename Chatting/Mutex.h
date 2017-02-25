/*************************************************************************
	> File Name: Mutex.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月25日 星期六 23时04分44秒
 ************************************************************************/

#ifndef _MUTEX_H
#define _MUTEX_H

#include<pthread.h>
#include<current.h>

//因为不是封装的库，所以在一些地方写的比较死，完全确认自己不会乱用

namespace Tool
{
    class MutexLock final
    {
        public:
            explicit MutexLock() : pid(0)
            {
                pthread_mutex_init(&mutex,NULL);
            };

            explicit ~MutexLock()
            {
                assert(!isLockByThisThread());
                pthread_mutex_destory(&mutex);
            }

            MutexLock(const MutexLock&) = delete;
            MutexLock& operator=(const MutexLock&) = delete;
            MutexLock(MutexLock&&) = delete;

            bool isLocedkByThisThread() 
            {
                return pid == CurrentThread::threadPid;
            };

            const pthread_t getPid()
            {
                return pid;
            }

            void lock()
            {
                pthread_mutex_lock(&mutex);
                pid = CurrentThread::threadPid;
            };

            pthread_mutex_t* getMutex()
            {
                return mutex;
            }

            void unlock();
            {
                assert(isLockByThisThread());
                pthread_mutex_unlock(&mutex);
                pid = 0;
            };

        private:
            pthread_t pid;
            pthread_mutex_t mutex;
    };
    
    class MutexLockGuard final
    {
        public:
            explicit MutexLockGuard(MutexLock& Mutex_) : mutex(mutex_)
            {
                mutex.lock();
            }

            explicit ~MutexLockGuard()
            {
                mutex.unlock();
            }
            
            MutexLockGuard(MutexLockGuard&) = delete;
            MutexLockGuard& operator=(const MutexLockGuard&) = delete;   
            MutexLockGuard(MutexLockGuard&&) = delete;

        private:
            MutexLock& mutex;
    };

}

#endif

