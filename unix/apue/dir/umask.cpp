/*************************************************************************
	> File Name: umask.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月11日 星期日 20时11分47秒
 ************************************************************************/

#include<iostream>
#include<fcntl.h>
#include"apue.h"
using namespace std;

#define RWRWRW S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH

int main(void)
{
    umask(0);
    if(creat("foo",RWRWRW) < 0)
        cerr << "foo creat error" << endl;
    umask(S_IROTH|S_IWOTH);
    if(creat("bar",RWRWRW) < 0)
        cerr << "bar creat error" << endl;
    exit(0);
}


