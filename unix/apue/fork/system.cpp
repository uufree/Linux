/*************************************************************************
	> File Name: system.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月17日 星期六 23时02分03秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
#include<sys/wait.h>
#include<errno.h>
#include<unistd.h>
using namespace std;

int system(const char* cmdstring)
{
    pid_t pid;
    int status;

    if(cmdstring == NULL)
        return 1;

    if((pid=fork()) < 0)
        status = -1;
    else if(pid==0)
    {
        excel("/bim/sh","sh","-c",cmdstring ,(char*)0);
        _exit(127);
    }
    else
    {
        while(waitpid(pid,&status,0) < 0)
        {
            if(errno!=EINTR)
            {
                status = -1;
                break;
            }
        }
    }
    return status;
}




