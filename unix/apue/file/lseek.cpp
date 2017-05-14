/*************************************************************************
	> File Name: lseek.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月09日 星期五 17时29分50秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
using namespace std;

int main(void)
{
    if(lseek(STDOUT_FILENO,0,SEEK_CUR) < 0)
        cout << "lseek error" << endl;
    else
        cout << "seek ok" << endl;

    exit(0);
}
