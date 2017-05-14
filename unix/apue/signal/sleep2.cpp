/*************************************************************************
	> File Name: sleep2.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月17日 星期六 23时44分32秒
 ************************************************************************/

#include<iostream>
#include<signal.h>
#include"apue.h"
#include<setjmp.h>
#include<unistd.h>
using namespace std;

static jmp_buf env;

static void sig_alrm(int signo)
{
    longjmp(env,1);
}

unsigned int sleep2(unsigned int seconds)
{
    if(signal(SIGALRM,sig_alrm) < 0)
        return seconds;
    if(setjmp(env) == 0)
    {
        alarm(seconds);
        pause();
    }
    return(alarm(0));
}

static void sig_int(int signo)
{
    int k=0;
    cout << endl << "signal starting" << endl;
    for(int i=0;i<300000;i++)
    {
        for(int j=0;j<4000;j++)
            k = k+i*j;
    }
    cout << "sig_int finished" << endl;
}

int main(void)
{
    unsigned int unslept;
    if(signal(SIGINT,sig_int) == SIG_ERR)
        cerr << "signal error" << endl;
    unslept = sleep2(5);
    cout << "sleep2 returned: " << unslept << endl;
    exit(0);
}





























