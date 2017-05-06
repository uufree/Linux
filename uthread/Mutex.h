/*************************************************************************
	> File Name: Mutex.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月25日 星期六 23时04分44秒
 ************************************************************************/

#ifndef _MUTEX_H
#define _MUTEX_H

#include<pthread.h>
#include<sys/syscall.h>
#include<unistd.h>

namespace now
{
    inline pid_t pid()
    {
        return ::syscall(SYS_gettid);
    }
}

namespace thread
{    
    class MutexLock final
    {
        public:
            explicit MutexLock() : pid(0)
            {
                pthread_mutex_init(&mutex,NULL);
            };
            
            ~MutexLock()
            {
                ::pthread_mutex_destroy(&mutex);
            }

            MutexLock(const MutexLock&) = delete;
            MutexLock& operator=(const MutexLock&) = delete;
            MutexLock(MutexLock&&) = delete;

            bool isLockInThisThread() const
            {
                return pid == now::pid();
            };
/*
            pid_t getPid() const
            {
                return pid;
            }
*/
            void lock()
            {
                ::pthread_mutex_lock(&mutex);
                pid = now::pid();
            };

            pthread_mutex_t* getMutex()
            {
                return &mutex;
            };

            void unlock()
            {
                pid = 0;
                ::pthread_mutex_unlock(&mutex);
            };
            

        private:
            pid_t pid;
            pthread_mutex_t mutex;
    };
    
    class MutexLockGuard final
    {
        public:
            explicit MutexLockGuard(MutexLock& mutex_) : mutex(mutex_)
            {
                mutex.lock();
            }

            ~MutexLockGuard()
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

