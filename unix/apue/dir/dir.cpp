/*************************************************************************
	> File Name: dir.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月11日 星期日 20时26分18秒
 ************************************************************************/

#include<iostream>
#include<dirent.h>
#include"apue.h"
using namespace std;

int main(int argc,char** argv)
{
    DIR* dp;
    struct dirent* drip;

    if(argc!=2)
    {
        cerr << "get informatiom error" << endl;
        return -1;
    }

    if((dp=opendir(argv[1])) == NULL)
        cerr << "get dir error" << endl;
    while((drip=readdir(dp)) != NULL)
        cout << drip->d_name << endl;

    closedir(dp);
    exit(0);
}
