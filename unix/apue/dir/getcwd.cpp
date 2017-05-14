/*************************************************************************
	> File Name: getcwd.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月11日 星期日 20时33分13秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
using namespace std;

int main()
{
    char* ptr = new char[100];
    if(getcwd(ptr,100) == NULL)
        cerr << "getcwd error" << endl;
    cout << ptr << endl;
    exit(0);
}
