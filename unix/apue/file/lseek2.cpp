/*************************************************************************
	> File Name: lseek2.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月09日 星期五 17时33分29秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
#include<fcntl.h>
using namespace std;

char buf1[] = "asdfghjkl";
char buf2[] = "ASDFGHJKL";

int main(void)
{
    int fd;

    if((fd=creat("file.hole",FILE_MODE)) < 0)
        cerr << "file creat error" << endl;
    if((write(fd,buf1,20)) != 20)
        cerr << "buf1 write error" << endl;
    if(lseek(fd,16384,SEEK_SET) < 0)
        cerr << "lseek error" << endl;
    if((write(fd,buf2,20)) != 20)
        cerr << "buf2 write error" << endl;

    exit(0);
}
