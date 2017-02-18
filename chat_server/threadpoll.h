/*************************************************************************
	> File Name: threadpoll.h
	> Author: 
	> Mail: 
	> Created Time: 2016年10月10日 星期一 19时05分36秒
 ************************************************************************/

#ifndef _THREADPOLL_H
#define _THREADPOLL_H

#include"chat.h"
#include"chat.cpp"

class ThreadPoll;

class Thread
{
    friend class ThreadPoll;
    friend class Server;
    private:
        pthread_t tid;
        static void* bios(void* arg)
        {
            int waitfd;
            int allfd = 0;
            Server server = (*(Server*)arg);
            int epollfd = epoll_create(256);

            while(1)
            { 
                epoll_event event;
                epoll_event eventss[256];

                pthread_rwlock_rdlock(&task);//得到需要处理的文件描述符
                if(task_list.empty())
                {
                    phread_rwlock_unlock(&task);
                    continue;
                }
                else
                {   
                    if(allfd < 256)
                    {
                        pthread_rwlock_wrlock(&task);
                        waitfd = task_list.front();
                        task_list.pop();
                        pthread_rwlock_unlock(&task);
                        pthread_rwlock_unlock(&task);
                        ++allfd;
                    }
                }
                event.data.fd = waitfd;//注册新的描述符
                event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                eopll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
        
                int ret = epoll_wait(epollfd,eventss,256,-1);
                if(ret < 0)
                continue;
                for(int i=0;i<ret;++i)
                {//epoll模型用来处理一次数据读取
                    server.handle_message(eventss[i].data.fd);
                    event.data.fd = waitfd;//重置epoll
                    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
                }
            }
        }
            
    public:
        Thread() {};
        int Start();
};

queue<int> task_list;//工作队列
pthraed_rwlock_t task = PTHREAD_RWLOCK_INITALIZER;
vector<Thread*> thread_list;//工作线程队列
pthread_rwlock_t thread = PTHREAD_RWLOCK_INITALIZER;

class ThreadPoll//线程共享数据只能放在外面，不能作为私有数据
{
    private:
        static int threadsize = 16;//最大线程数
        static int thread_user = 256;//每个线程处理的最多客户数
    public:
        ThreadPoll(){};
        ~ThreadPoll();
        int Start();//创建线程对象
}



#endif
