/**********************************************************************
	> File Name: pressure_test.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年10月12日 星期三 23时35分23秒
 *********************************************************************/

#include"chat.h"
#include"chat.cpp"
using namespace std;

int main()
{
    string name,password;
    char ch;

    for(int i=0;i<5000;i++)
    {
        ch = i;
        name = ch;
        password = ch;
        Client client(name,password);
    }

    exit(0);
}

