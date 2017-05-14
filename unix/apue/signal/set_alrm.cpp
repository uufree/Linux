/*************************************************************************
	> File Name: set_alrm.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月18日 星期日 00时36分27秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
#include<setjmp.h>
using namespace std;

static void sig_alrm(int);
static jmp_buf env;

int main()
{
    int n;
    char line[MAXLINE];

    if(signal(SIGALRM,sig_alrm) == SIG_ERR)
        cerr << "signal error" << endl;
    if(setjmp(env) != 0)
        cerr << "read timeout" << endl;

    alarm(10);
    if((n=read(STDIN_FILENO,line,MAXLINE)) < 0)
        cerr << "read error" << endl;
    alarm(0);

    write(STDOUT_FILENO,line,n);
    exit(0);
}

static void sig_alrm(int sinno)
{
    longjmp(env,1);
}

