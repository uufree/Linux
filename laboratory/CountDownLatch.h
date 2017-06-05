/*************************************************************************
	> File Name: CountDownLatch.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月27日 星期一 17时46分45秒
 ************************************************************************/

#ifndef _COUNTDOWNLATCH_H
#define _COUNTDOWNLATCH_H

#include"Mutex.h"
#include"Condition.h"

namespace unet
{
    namespace thread
    {
        class CountDownLatch final
        {
            public:
                explicit CountDownLatch(int size) : mutex(),notzero(mutex),count(size) {};

                CountDownLatch(const CountDownLatch&) = delete;
                CountDownLatch(CountDownLatch&&) = delete;
                CountDownLatch& operator=(const CountDownLatch&) = delete;

                void countDown()
                {
                    MutexLockGuard guard(mutex);
                    --count;
                    if(count == 0)
                    {
                        notzero.notifyAll();
                    }
                }

                void wait()
                {
                    MutexLockGuard guard(mutex);
                    while(count > 0)
                    {
                        notzero.wait();
                    }
                }

                int getCount() const
                {
                    MutexLockGuard guard(mutex);
                    return count;
                }
    
            private:
                mutable MutexLock mutex;
                Condition notzero;
                int count;
        };
    }
}


#endif

