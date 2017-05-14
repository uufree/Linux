/*************************************************************************
	> File Name: compete.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月14日 星期三 00时56分17秒
 ************************************************************************/


//这是个锤子！！



#include<iostream>
#include"apue.h"
using namespace std;


int main(void)
{
    pid_t pid;

    if((pid=fork()) < 0)
        cerr << "fork error" << endl;
    else if(pid==0)
        cout << "child" << endl;
    else
        cout << "parent" << endl;
    exit(0);
}

