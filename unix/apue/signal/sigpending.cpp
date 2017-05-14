/*************************************************************************
	> File Name: sigpending.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月18日 星期日 00时49分20秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
using namespace std;

static void sig_quit(int);

int main()
{
    sigset_t newmask,oldmask,pendmask;

    if(signal(SIGQUIT,sig_quit) == SIG_ERR)
        cerr << "signal error" << endl;

    sigemptyset(&newmask);
    sigaddset(&newmask,SIGQUIT);
    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0)
        cerr << "sig_block error" << endl;

    sleep(5);

    if(sigpending(&pendmask) < 0)
        cerr << "sigpending  error" << endl;
    if(sigismember(&pendmask,SIGQUIT))
        cout << "SIGQUIT pending" << endl;

    if(sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
        cerr << "SIG_SETMASK error" << endl;
    cout << "SIGQUIT unblocked" << endl;

    sleep(5);
    exit(0);
}

static void sig_quit(int signo)
{
    cout << "caught SIGQUIT" << endl;
    if(signal(SIGQUIT,SIG_DFL) == SIG_ERR)
        cerr << "can't reset SIGQUIT" << endl;
}

