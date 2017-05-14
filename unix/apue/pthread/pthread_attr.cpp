/*************************************************************************
	> File Name: pthread_attr.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月19日 星期一 02时06分46秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
#include<pthread.h>
using namespace std;

int makethread(void* (*fn)(void*),void* arg)
{
    int err;
    pthread_t tid;
    pthread_attr_t attr;

    err = pthread_attr_init(&attr);
    if(err!=0)
        return err;
    err = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    if(err == 0)
        err  = pthread_create(&tid,&attr,fn,arg);
    pthread_attr_destory(&attr);
    return err;
}



