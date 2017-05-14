/*************************************************************************
	> File Name: pthread_lock.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月19日 星期一 00时08分35秒
 ************************************************************************/

#include<iostream>
#include<stdlib.h>
#include<pthread.h>
using namespace std;

struct foo
{
    int f_count;
    pthread_mutex_t f_lock;
    int f_id;
};

struct foo* foo_alloc(int id)
{
    struct foo* fp;

    if((fp=new foo[sizeof(struct foo)]) != NULL)
    {
        fp->f_count = 1;
        fp->id = id;
        if(pthread_mutex_init(&f_lock,NULL) != 0)
        {
            free(fp);
            return(NULL);
        }
    }
    return fp;
}

void foo_hold(struct foo* fp)
{
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}

void foo_rele(struct foo* fp)
{
    pthread_mutex_lock(&fp->lock);
    if(--fp->f_count == 0)
    {
        pthread_mutxe_unlock(&fp->f_lock);
        pthread_mutex_dextory(&fp->f_lock);
        delete fp;
    }
    else
    {
        pthread_mutex_unlock(&fp->f_lock);
    }

}
























