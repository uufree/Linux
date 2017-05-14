/*************************************************************************
	> File Name: fork.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月14日 星期三 00时21分41秒
 ************************************************************************/

//演示子进程与父进程不共享数据

#include<iostream>
#include"apue.h"

using namespace std;

int globar = 6;
char buf[] = "asdfghjkl";

int main(void)
{
    int var;
    pid_t pid;

    var = 88;
    if(write(STDOUT_FILENO,buf,sizeof(buf)-1) != (sizeof(buf)-1))
        cerr << "write error" << endl;

    cout << "before fork: " << endl;

    if((pid=fork()) < 0)
        cerr << "fork error" << endl;
    else if(pid==0)
    {
        globar++;
        var++;
    }
    else
        sleep(2);

    cout << getpid() << endl << globar << endl << var << endl;
    exit(0);
}
