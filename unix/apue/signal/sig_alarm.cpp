/*************************************************************************
	> File Name: sig_alarm.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月18日 星期日 00时23分23秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
using namespace std;

static void sig_alrm(int);

int main()
{
    int n;
    char line[MAXLINE];
    if(signal(SIGALRM,sig_alrm) == SIG_ERR)
        cerr << "signal(SIGALRM) error" << endl;

    alarm(10);
    if((n=read(STDIN_FILENO,line,MAXLINE)) < 0)
        cerr << "read error" << endl;
    alarm(0);

    write(STDOUT_FILENO,line,n);
    exit(0);
}

static void sig_alrm(int signo)
{
}
