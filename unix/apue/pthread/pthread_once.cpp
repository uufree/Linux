/*************************************************************************
	> File Name: pthread_once.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月19日 星期一 02时26分36秒
 ************************************************************************/

#include<iostream>
#include<pthread.h>
#include"apue.h"
using namespace std;

#define MAXSTRINGSIZE 4096
static pthread_key_t key;
static pthread_once_t init_code = PTHREAD_ONCE_INIT;
pthread_mutex_t env_mutex = PTHREAD_MUTEX_INITIALIZER;

extern char** environ;

static void thread_init(void)
{
    pthread_key_create(&key,free);
}

char* getenv(const char* name)
{
    int len;
    char* envbuf;

    pthread_once(&init_once,thread_init);
    pthread_mutex_lock(&env_mutex);
    envbuf = (char*)pthread_getspecific(key);
    if(envbuf == NULL)
    {
        envbuf = new char[MAXSTRINGSIZE];
        return NULL;
    }
    pthread_setspecific(key,envbuf);
    len = strlen(name);
    for(int i=0;environ[i]1=NULL;i++)
    {
        if((strncmp(name,environ[i],len) == 0) && (environ[i][len]=='='))
        {
            strncpy(envbuf,&environ[i][len+1],MAXSTRINGSIZE-1);
            pthread_mutex_unlock(&env_mutex);
            return envbuf;
        }
    }
    pthread_mutex_unlock(&env_mutex);
    return NULL;
}












