/*************************************************************************
	> File Name: link.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月11日 星期日 20时21分08秒
 ************************************************************************/

#include<iostream>
#include<fcntl.h>
#include"apue.h"

using namespace std;

int main(void)
{
    if(open("foo",O_RDWR) < 0)
        cerr << "open error" << endl;

    if(unlink("foo") < 0)
        cerr << "unlink error" << endl;

    cout << "file unlink" << endl;

    sleep(2);
    cout << "done" << endl;
    exit(0);
}
