/*************************************************************************
	> File Name: wait.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月14日 星期三 00时31分44秒
 ************************************************************************/

#include<iostream>
#include<sys/wait.h>
#include"apue.h"
using namespace std;

void pr_exit(int status)
{
    if(WIFEXITED(status))
        cout << "子进程正常终止" << endl;
    else if(WIFSIGNALED(status))
        cout << "子进程异常终止" << endl;
    else if(WIFSTOPPED(status))
        cout << "子进程当前停止" << endl;
    else if(WIFCONTINUED(status))
        cout << "暂停后继续的子进程" << endl;

}

int main(void)
{
    pid_t pid;
    int status;

    if((pid=fork()) < 0)
        cerr << "forh error" << endl;
    else if(pid==0)
        exit(7);

    if(wait(&status) != pid)
        cerr << "wait error" << endl;
    pr_exit(status);

    if((pid=fork()) < 0)
        cerr << "fork error" << endl;
    else if(pid==0)
        abort();

    if(wait(&status) != pid)
        cerr << "wait error" << endl;
    pr_exit(status);

    if((pid=fork()) < 0)
        cerr << "fork error" << endl;
    else if(pid==0)
        status = 0;

    if(wait(&status) != pid)
        cerr << "wait error" << endl;
    pr_exit(status);

    exit(0);
}




























