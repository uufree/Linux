/*************************************************************************
	> File Name: signal.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月14日 星期三 01时07分58秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
#include<signal.h>
using namespace std;

static volatile sig_atomic_t sigflag;
static sigset_t newmask,oldmask,zeromask;

static void sig_usr(int signo)
{
    sigflag = 1;
}

void TELL_WAIT(void)
{
    if(signal(SIGUSR1,sig_usr) == SIG_ERR)
        cerr << "signal error" << endl;

    if(signal(SIGUSR2,sig_usr) == SIG_ERR)
        cerr << "signal error" << endl;
    sigemptyset(&zeromask);//没有屏蔽任何东西，也就是说这个过程中如果产生其他信号，立即停止等待。
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

int main(void)
{
    pid_t pid;
    TELL_WAIT();

    if((pid=fork()) < 0)
        cerr << "fork error" << endl;
    else if(pid==0)
    {
        WAIT_PARENT();
        cout << "output fron child" << endl;
    }
    else
    {
        cout << "output from parent" << endl;
        TELL_CHILD(pid);
    }
    exit(0);
}





































