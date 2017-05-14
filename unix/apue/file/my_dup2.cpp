/*************************************************************************
	> File Name: my_dup2.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月11日 星期日 19时27分22秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
#include<fcntl.h>
#include<vector>
using namespace std;

int mydup2(int fd,int newfd);

int main()
{
    int newfd;
    cin >> newfd;
    newfd = mydup2(0,newfd);
    cout << newfd << endl;
   
    exit(0);
}

int mydup2(int fd,int newfd)
{
    if(fd<0 || fd>256)
    {
        cerr << "fd error" << endl;
        return -1;
    }

    if(newfd<0 || newfd>256)
    {
        cerr << "newfd error" << endl;
        return -1;
    }

    if(newfd == fd)
        return newfd;

    if(dup(fd) == -1)
    {
        cerr << "fd not open" << endl;
        return -1;
    }

    int newfdindex;
    vector<int> vi;
    while((newfdindex=dup(fd)) != newfd)
        vi.push_back(newfdindex);

    int size = vi.size();
    for(int i=0;i<size;++i)
        close(vi[i]);

    return newfdindex;
}


    



