/*************************************************************************
	> File Name: Thread.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月27日 星期一 23时27分28秒
 ************************************************************************/

#include"Thread.h"
#include<pthread.h>

namespace unet
{
    namespace thread
    {
        void* runInThread(void* arg)
        {
            typedef std::function<void ()> ThreadFunc;
            ThreadFunc* thread(static_cast<ThreadFunc*>(arg));

            (*thread)();
            return 0;
        };
       
        Thread::Thread(const Thread& lhs)
        {
            threadfunc = lhs.threadfunc;
            threadid = lhs.threadid;
        }

        Thread::Thread(Thread&& lhs)
        {
            
            threadfunc = lhs.threadfunc;
            threadid = lhs.threadid;
        }

        Thread& Thread::operator=(const Thread& lhs)
        {

            this->threadfunc = lhs.threadfunc;
            this->threadid = lhs.threadid;
            return *this;
        }

        Thread& Thread::operator=(Thread&& lhs)
        {

            this->threadfunc = lhs.threadfunc;
            this->threadid = lhs.threadid;
            return *this;
        }

        Thread::~Thread()
        {
            ::pthread_detach(threadid);
        }

        int Thread::start()
        {
            return pthread_create(&threadid,NULL,unet::thread::runInThread,&threadfunc);
        }

        int Thread::join()
        {
            return pthread_join(threadid,NULL);
        }
    }
}

