/*************************************************************************
	> File Name: read_write.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月09日 星期五 17时40分52秒
 ************************************************************************/

#include<iostream>
#include<fcntl.h>
#include"apue.h"
using namespace std;

#define BUFSIZE 4096

int main(void)
{
    int n;
    char buf[BUFSIZE];

    while((n=read(STDIN_FILENO,buf,BUFSIZE)) > 0)
        if(write(STDOUT_FILENO,buf,n) != n)
            cerr << "write error" << endl;

    if(n<0)
        cerr << "read error" << endl;

    exit(0);
}
    
