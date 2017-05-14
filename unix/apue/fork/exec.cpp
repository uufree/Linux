/*************************************************************************
	> File Name: exec.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月17日 星期六 22时47分57秒
 ************************************************************************/

#include<iostream>
#include<sys/wait.h>
#include"apue.h"
using namespace std;

char* env[] = {"USER=unknow","PATH=/tmp",NULL};

int main()
{
    pid_t pid;

    if((pid=fork()) < 0)
        cerr << "fork error" << endl;
    else if(pid==0)
    {
        if(execl("/home/will/unix/fork/hello","hello",(char*)0,env) < 0)
            cerr << "execl error"<< endl;
        if(waitpid(pid,NULL,0) < 0)
            cerr << "wait error" << endl;
    }

    exit(0);
}

