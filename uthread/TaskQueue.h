/*************************************************************************
	> File Name: TaskQueue.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月27日 星期一 17时12分34秒
 ************************************************************************/

#ifndef _TASKQUEUE_H
#define _TASKQUEUE_H

#include<deque>
#include<assert.h>
#include"Mutex.h"
#include"Condition.h"

//多线程任务队列神器
namespace thread
{
    template<typename T>
    class TaskQueue final
    {
        public:
            explicit TaskQueue() : mutex(),notempty(mutex),queue() {};

            TaskQueue(const TaskQueue&) = delete;
            TaskQueue(TaskQueue&&) = delete;
            TaskQueue& operator=(const TaskQueue&) = delete;
            ~TaskQueue() {};

            void put(const T& task)
            {
                MutexLockGuard guard(mutex);
                queue.push_back(task);
                notempty.notify();
            }

//将C++11的move特性与旧版的区别开来            
#ifdef __GXX_EXPERIMENTAL_CXX0X__
            void put(T&& task)//调用者使用std::move
            {
                MutexLockGuard guard(mutex);
                queue.push_back(task);
                notempty.notify();
            }
#endif                
            
            T get()
            {
                MutexLockGuard guard(mutex);
                
                while(queue.empty())
                {
                    notempty.wait();
                }
                assert(!queue.empty());

#ifdef __GXX_EXPERIMENTAL_CXX0X__
                T retu(std::move(queue.front()));
#else
                T retu(queue.front());
#endif
                queue.pop_front();
                return retu;
            }

            size_t size()
            {
                MutexLockGuard guard(mutex);
                return queue.size();
            }

        private:
            std::deque<T> queue;
            mutable MutexLock mutex;
            Condition notempty;
    };
}






#endif

