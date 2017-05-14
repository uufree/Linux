/*************************************************************************
	> File Name: stat.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月11日 星期日 19时53分07秒
 ************************************************************************/

#include<iostream>
#include"apue.h"

using namespace std;

int main(int argc,char** argv)
{   
    struct stat buf;
    
    if(argc!=2)
    {
        cerr << "get error" << endl;
        return -1;
    }

    for(int i=0;i<2;i++)
    {
        if((lstat(argv[i],&buf)) < 0)
        {
            cerr << "lstat get error" << endl;
            return -1;
        }
        
        if(S_ISREG(buf.st_mode))
            cout << "regular" << endl;
        else if(S_ISDIR(buf.st_mode))
            cout << "directory" << endl;
        else if(S_ISCHR(buf.st_mode))
            cout << "character special" << endl;
        else if(S_ISBLK(buf.st_mode))
            cout << "block special" << endl;
        else if(S_ISFIFO(buf.st_mode))
            cout << "fifo" << endl;
        else if(S_ISLNK(buf.st_mode))
            cout << "symbolic link" << endl;
        else if(S_ISSOCK(buf.st_mode))
            cout << "socket" << endl;
        else
            cout << "unknown mode" << endl;
    }
    exit(0);
}

            
    
