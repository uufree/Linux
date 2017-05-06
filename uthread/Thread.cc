/*************************************************************************
	> File Name: Thread.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月27日 星期一 23时27分28秒
 ************************************************************************/

#include"Thread.h"
#include<iostream>

namespace thread
{
    void* runInThread(void* arg)
    {
        ThreadFunc* thread(static_cast<ThreadFunc*>(arg));

        (*thread)();
        return 0;
    };
       
    Thread::~Thread()
    {
        if(isstart)
            assert(::pthread_detach(threadid) == 0 && threadid != 0);
    }

    void Thread::start()
    {
        if(!isstart)
        {
             assert(::pthread_create(&threadid,NULL,runInThread,&threadfunc) == 0);
             isstart = true;
        }
    }

    void Thread::join()
    {
        if(isstart)
        {
            assert(::pthread_join(threadid,NULL) == 0 && threadid != 0);
            threadid = 0;
            isstart = false;
        }
    }
}

