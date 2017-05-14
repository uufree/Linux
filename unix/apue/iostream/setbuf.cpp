/*************************************************************************
	> File Name: setbuf.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月11日 星期日 21时36分34秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
using namespace std;

int mysetbuf(FILE* fp,char* restrict buf)
{
    if(buf == NULL)
    {
        if(setvbuf(fp,buf,_IOFBF,BUFSIZ) != 0)
        cerr << "no buf" << endl;
        return -1;
    }
    else
    {
        if(fp == stderr)
        {
           if(setvbuf(fp,buf,_IONBF,BUFSIZ) != 0)
           {
               cerr << "setvbuf error" << endl;
               return -1;
           }
        }
        else if(fp==stdout || fp==stdin)
        {
            if(setvbuf(fp,buf,_IOLBF,BUFSIZ) != 0)
            {
                cerr << "setvbuf error" << endl;
                return -1;
            }
        }
        else
        {
            if(setvbuf(fp,buf,_IOFBF,BUFSIZ) != 0)
            {
                cerr << "setvbuf error" << endl;
                return -1;
            }
        }
    }
}
            
