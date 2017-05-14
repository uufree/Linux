/*************************************************************************
	> File Name: tell.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月18日 星期日 04时59分56秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
using namespace std;

static volatile sig_atomic_t sigflag;
static sigset_t newmask,oldmask,zeromask;

static void sig_usr(int signo)
{
    sigflag = 1;
}

void TELL_WAIT()
{
    if(signal(SIGUSR1,sig_usr) == SIG_ERR)
        cerr << "signal(SIGUSR1) error" << endl;
    if(signal(SIGUSR2,sig_usr) == SIG_ERR)
        cerr << "signal(SIGUSR2) error" << endl;
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGUSR1);
    sigaddset(&newmask,SIGUSR2);

    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0)
        cerr << "SIG_BLOCK error" << endl;
}

void TELL_PARENT(pid_t pid)
{
    kill(pid,SIGUSR2);
}

void TELL_CHILD(pid_t pid)
{
    kill(pid,SIGUSR1);
}

void WAIT_PARENT(void)
{
    while(sigflag == 0)
        sigsuspend(&zeromask);
    sigflag = 0;

    if(sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
        cerr << "SIG_SETMASK error" << endl;
}

void WAIT_CHILD(void)
{
    while(sigflag == 0)
        sigsuspend(&zeromask);
    sigflag = 0;

    if(sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
        cerr << "SIG_SETMASK error" << endl;
}


















