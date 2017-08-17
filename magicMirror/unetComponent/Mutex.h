/*************************************************************************
	> File Name: Mutex.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月25日 星期六 23时04分44秒
 ************************************************************************/

//封装互斥锁


#ifndef _MUTEX_H
#define _MUTEX_H

#include<pthread.h>
#include<sys/syscall.h>
#include<unistd.h>
#include"error.h"

namespace unet
{
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
                explicit MutexLock();
                MutexLock(const MutexLock&) = delete;
                MutexLock& operator=(const MutexLock&) = delete;
                MutexLock(MutexLock&& lhs);
                MutexLock& operator=(MutexLock&& lhs);
                ~MutexLock();
                    
                void swap(MutexLock& lhs) = delete;

                inline bool isLockInThisThread() const
                {return pid==now::pid();};
                
                inline pthread_mutex_t& getMutex()
                {return mutex;};
                
                inline void lock();
                inline void unlock();
            
            private:
                pid_t pid;
                pthread_mutex_t mutex;
        };
    
        class MutexLockGuard final
        {
            public:
                explicit MutexLockGuard(MutexLock& mutex_);
                ~MutexLockGuard();
            
                MutexLockGuard(MutexLockGuard&) = delete;
                MutexLockGuard& operator=(const MutexLockGuard&) = delete;   
                MutexLockGuard(MutexLockGuard&&) = delete;
                MutexLockGuard& operator=(MutexLockGuard&&) = delete;
                void swap(MutexLockGuard& lhs) = delete;

            private:
                MutexLock& mutex;
        };
    }
}

#endif

