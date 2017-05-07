/*************************************************************************
	> File Name: Thread.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月26日 星期日 00时35分15秒
 ************************************************************************/

#ifndef _THREAD_H
#define _THREAD_H

#include<pthread.h>
#include<functional>
#include<assert.h>
#include<iostream>

namespace thread
{
    typedef std::function<void()> ThreadFunc;
    void* runInThread(void* arg);

    class Thread final
    {
        public:
            explicit Thread() : threadid(0),
            isstart(false)
            {};
            
            explicit Thread(const ThreadFunc& lhs) : threadid(0),
            isstart(false),
            threadfunc(lhs)
            {};
           
            Thread(const Thread& lhs);
            Thread(Thread&& lhs);
            Thread& operator=(const Thread& lhs);
            
            ~Thread();
                
            void start();
            void join();
            
            bool isStart() const 
            {return isstart;};

            void setThreadCallBack(const ThreadFunc& cb)
            {threadfunc = cb;};

            pthread_t getThreadId() const 
            {return threadid;};  

        private:
            pthread_t threadid;
            bool isstart;
            ThreadFunc threadfunc;
    };

}
                        
#endif

