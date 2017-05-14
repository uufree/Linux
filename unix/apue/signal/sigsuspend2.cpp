/*************************************************************************
	> File Name: sigsuspend2.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月18日 星期日 04时47分18秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
using namespace std;

volatile sig_atomic_t quitflag;

static void sig_int(int signo)
{
    if(signo == SIGINT)
        cout << "interrupt" << endl;
    else if(signo == SIGQUIT)
        quitflag = 1;
}

int main()
{
    sigset_t newmask,oldmask,zeromask;

    if(signal(SIGINT,sig_int) == SIG_ERR)
        cerr << "signal(sigint) error" << endl;
    if(signal(SIGQUIT,sig_int) == SIG_ERR)
        cerr << "signal(sigquit) error" << endl;

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGQUIT);

    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0)
        cerr << "SIG_BLOCK error" << endl;

    while(quitflag == 0)
        sigsuspend(&zeromask);

    quitflag = 0;

    if(sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
        cerr << "SIG_SETMASK error" << endl;

    exit(0);
}
        
























