//能用对象控制线程，但是线程运行函数必须在类私有部分，为静态
//所有线程（对象）共享数据必须全局变量

#include"threadpoll.h"

/*******************Thread函数************************/
int Thread::Start()
{
    Server server;
    pthread_attr_t attr;
    err = phread_attr_init(&attr);
    if(err != 0)
    {
        cerr << "pthread attr create error" << endl;
        exit(0);
    }
    err = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHD);
    if(err != 0)
    {
        cerr << "attr create error" << endl;
        exit(0);
    }
    int err = pthread_create(&tid,&attr,bios,&server);
    if(err != 0)
    {
        cerr << "pthread create error" << endl;
        exit(0);
    }
}


/******************ThreadPoll函数******************w****/
int ThreadPoll::Start()
{
    for(int i=0;i<threadsize;i++)
    {
        Thread* thr = new Thread;
        thr->Start();
        thread_list.push_back(thr);
    }
    return 0;
}

ThreadPoll::~ThreadPoll()
{
    phread_rwlock_wrlock(&thread);
    for(int i=0;i<threadsize;i++)
        delete thread_list[i];
    pthread_rwlock_unlock(&thread);
}

            

    




















