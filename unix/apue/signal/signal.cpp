/*************************************************************************
	> File Name: signal.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月17日 星期六 23时14分49秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
using namespace std;

static void sig_usr(int);

int main(void)
{
    if(signal(SIGUSR1,sig_usr) < 0)
        cerr << "can't catch SIGNAL1" << endl;
    if(signal(SIGUSR2,sig_usr) < 0)
        cerr << "can't catch SIGNAL2" << endl;

    for(;;)
        pause();
}

static void sig_usr(int signo)
{
    if(signo==SIGUSR1)
        cout << "receive SIGUSR1" << endl;
    else if(signo==SIGUSR2)
        cout << "receive SIGUSR2" << endl;
    else
        cerr << "receive " << signo << endl;
}
