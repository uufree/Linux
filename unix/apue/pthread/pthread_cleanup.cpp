/*************************************************************************
	> File Name: pthread_cleanup.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月18日 星期日 05时59分32秒
 ************************************************************************/

#include<iostream>
#include<pthread.h>
#include"apue.h"
using namespace std;

void cleanup(void* arg)
{
    cout << "cleanup: " <<(char*)arg << endl;
}

void* thr_fn1(void* arg)
{
    cout << "thread 1 start: " << endl;
    pthread_cleanup_push(cleanup,"thread 1 first handler");
    pthread_cleanup_push(cleanup,"thread 1 second hander");
    cout << "pthread 1 push complete" << endl;
    if(arg)
        return (void*)1;
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return (void*)1;
}

void* thr_fn2(void* arg)
{
    cout << "thread 2 start: " << endl;
    pthread_cleanup_push(cleanup,"thread 2 first handler");
    pthread_cleanup_push(cleanup,"thread 2 second hander");
    cout << "pthread 2 push complete" << endl;
    if(arg)
        return (void*)1;
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    pthread_exit((void*)2);
}

int main(void)
{
    int err;
    pthread_t tid1,tid2;
    void* tret;

    err = pthread_create(&tid1,NULL,thr_fn1,NULL);
    if(err!=0)
        cerr << "thread 1 create error" << endl;
    err = pthread_create(&tid2,NULL,thr_fn2,NULL);
    if(err!=0)
        cerr << "thrtead 2 create error" << endl;

    err = pthread_join(tid1,&tret);
    if(err!=0)
        cerr << "can't join with thread 1" << endl;
    cout << "thread 1 exit code: " << &tret << endl;
    err = pthread_join(tid2,&tret);
    if(err!=0)
        cerr << "can't join with thread 2" << endl;
    cout << "thread 2 exit code: " << &tret << endl;
    exit(0);
}















