/*************************************************************************
	> File Name: sigsuspend.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月18日 星期日 02时09分02秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
using namespace std;

static void sig_int(int);

int main()
{
    sigset_t newmask,oldmask,waitmask;
    
    if(signal(SIGINT,sig_int) == SIG_ERR)
        cerr << "signal(sig_int) error" << endl;
    sigemptyset(&waitmask);
    sigaddset(&waitmask,SIGUSR1);
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGINT);

    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0)
        cerr << "SIG_BLOCK error" << endl;

    if(sigsuspend(&waitmask) != -1)//将参数中的信号加入屏蔽字，然后恢复信号屏蔽字，然后再将进程挂起
        cerr << "sigsuspend error" << endl;

    if(sigprocmask(SIG_BLOCK,&oldmask,NULL) < 0)
        cerr << "SIG_BLOCK error" << endl;

    exit(0);
}

static void sig_int(int signo)
{

}

