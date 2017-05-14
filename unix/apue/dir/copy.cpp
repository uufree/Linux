/*************************************************************************
	> File Name: copy.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月11日 星期日 20时41分13秒
 ************************************************************************/

#include<iostream>
#include<fcntl.h>
#include"apue.h"
using namespace std;

int main(int argc,char** argv)
{
    if(argc!=3)
    {
        cerr << "get error" << endl;
        return -1;
    }

    int fd1,fd2;
    if((fd1=openat(AT_FDCWD,argv[1],O_RDONLY)) < 0)
        cerr << "fd1 open error" << endl;
    if((fd2=openat(AT_FDCWD,argv[2],O_RDWR|O_APPEND|O_TRUNC)) < 0)
        cerr << "fd2 open error" << endl;

    char* buf = new char[1];
    while(read(fd1,buf,1) != 0)
    {
        if(*buf != '\0')
            write(fd2,buf,1);
    }

    close(fd1);
    close(fd2);
    exit(0);
}


