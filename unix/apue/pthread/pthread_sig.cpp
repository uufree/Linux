/*************************************************************************
	> File Name: pthread_sig.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月19日 星期一 02时46分55秒
 ************************************************************************/

#include<iostream>
#include<pthread.h>
#include"apue.h"
using namespace std;

int quitflag;
sigset_t mask;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;

void* thr_fn(void* arg)
{
    int err,signo;

    for(;;)
    {
        err = sigwait(&mask,&signo);
        if(err != 0)
            cerr << "sigwait failed" << endl;
        switch(signo)
        {
            case SIGINT: 
                cout << "interrupt" << endl;
                break;

            case SIGQUIT:
                pthread_mutex_lock(&lock);
                qiutflag = 1;
                pthread_mutex_unlock(&lock);
                pthread_cond_siganl(&waitloc);
                return 0;
                
            default:
                cout << "can't expect signal" << endl;
                exit(1);
        }
    }
}

int main()
{
    int err;
    sigset_t oldmask;
    pthread_t tid;

    sigemptyset(&mask);
    sigaddset(&mask,SIGQUIT);
    sigaddset(&mask,SIGINT);

    if((err=pthread_sigmask(SIG_BLOCK,&mask,&oldmask)) != 0)
        cerr << "SIG_BLOCK error" << endl;

    err = pthread_create(&tid,NULL,thr_fn,0);
    if(err!=0)
        cerr << "pthraed_create error" << endl;

    pthread_mutex_lock(&lock);
    while(quitflag == 0)
        pthread_cond_wait(&waitloc,&lock);
    pthread_mutex_unlock(&lock);

    quitflag = 0;

    if(sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
        cerr << "SIG_SETMASK error" << endl;
    exit(0);
}




                














