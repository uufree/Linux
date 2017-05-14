/*************************************************************************
	> File Name: waitpid.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月14日 星期三 00时46分13秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
#include<sys/wait.h>
using namespace std;

int main(void)
{
    pid_t pid;

    if((pid=fork()) < 0)
        cerr << "fork error" << endl;
    else if(pid==0)
    {
        if((pid=fork()) < 0)
            cerr << "fork error" << endl;
        else if(pid>0)
            exit(0);
        sleep(2);
        cout << "father pid: " << getppid() << endl;
        exit(0);

    }
    if(waitpid(pid,NULL,0) != pid)
        cerr << "waitpid error" << endl;
    exit(0);
}


