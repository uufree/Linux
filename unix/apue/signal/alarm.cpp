/*************************************************************************
	> File Name: alarm.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月17日 星期六 23时31分48秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
#include<pwd.h>
using namespace std;

static void my_alarm(int signo)
{
    struct passwd* rootptr;

    cout << "in signal handler: " << endl;
    if((rootptr=getpwnam("root")) == NULL)
        cerr << "getpwnam error" << endl;
    alarm(1);
}

int main()
{
    struct passwd* ptr;
    signal(SIGALRM,my_alarm);
    alarm(1);
    for(;;)
    {
        if((ptr=getpwnam("sar")) == NULL)
            cerr << "getpwnam error" << endl;
        if(strcmp(ptr->pw_name,"sar") != 0)
            cout << "return value corrupted! pw_name=" << ptr->pw_name << endl;
    }
}
