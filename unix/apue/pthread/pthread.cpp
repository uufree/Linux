/*************************************************************************
	> File Name: pthread.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月18日 星期日 05时28分10秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
#include<pthread.h>
using namespace std;

//线程库不是G++标准库，在编译时加上  -lpthread

pthread_t ntid;

void printids(const char* s)
{
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();

    cout << "fork: " << pid << endl << "pthread: " << tid << endl;
}

void* thr_fn(void* argv)
{
    printids("new pehread: ");
    return (void*)0;
}

int main()
{
    int err;

    err = pthread_create(&ntid,NULL,thr_fn,NULL);
    if(err!=0)
        cerr << "can't create pthread" << endl;

    printids("main pthread: ");
    sleep(1);
    exit(0);
}


















