/*************************************************************************
	> File Name: pthraed_attr1.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月19日 星期一 02时14分19秒
 ************************************************************************/

#include<iostream>
#include<pthread.h>
#include"apue.h"
using namespace std;

pthread_mutexattr_t attr;
pthread_mutex_mutex;

imnt main()
{
    int err,condiation,arg;
    
    if((err=pthread_mutexattr_init(&attr)) != 0)
        cerr << "pthread_mutexattr_init failed" << endl;

    if((err=pthread_mutexattr_settype(&attr,PTHREAD_MUTXE_RECURSIVE)) != 0)
        cerr << "can't set recurive type" << endl;

    if((err=pthread_create(&mutex,&attr)) != 0)
        cerr << "can't create recursive mutex";

    pthread_mutex_lock(&mutex);


    pthread_mutex_unlock(&mutex);

    exit(0);
}




