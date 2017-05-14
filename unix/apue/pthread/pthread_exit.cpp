/*************************************************************************
	> File Name: pthread_exit.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月18日 星期日 05时43分38秒
 ************************************************************************/

#include<iostream>
#include<pthread.h>
#include"apue.h"
using namespace std;

void* thr_fn1(void* arg)
{
    cout << "thread 1 returning" << endl;
    return (void*)0;
}

void* thr_fn2(void* arg)
{
    cout << "thread 2 exiting" << endl;
    exit(0);
}

int main()
{
    int err;
    pthread_t tid1,tid2;
    void* tret;

    err = pthread_create(&tid1,NULL,thr_fn1,NULL);
    if(err != 0)
        cerr << "can't create thread 1" << endl;
    err = pthread_create(&tid2,NULL,thr_fn2,NULL);
    if(err!=0)
        cerr << "can't create thread 2" << endl;
    
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
















