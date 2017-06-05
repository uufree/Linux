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


namespace unet
{
    namespace thread
    {
        void* runInThread(void* arg);

        class Thread final
        {
            typedef std::function<void()> ThreadFunc;
            
            public:
                explicit Thread() : threadid(0)
                {};
                
                explicit Thread(const ThreadFunc& lhs) : threadid(0)
                {threadfunc = lhs;};

                Thread(const Thread& lhs);
                Thread(Thread&& lhs);
                Thread& operator=(const Thread& lhs);
                Thread& operator=(Thread&& lhs);
                ~Thread();
                
                int start();
                int join();

                void setThreadCallBack(const ThreadFunc& cb)
                {threadfunc = cb;};

                pthread_t getThreadId()
                {return threadid;};  

            private:
                pthread_t threadid;
                ThreadFunc threadfunc;
        };
    }

}
            
            
#endif

