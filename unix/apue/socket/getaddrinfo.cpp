/*************************************************************************
	> File Name: getaddrinfo.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月20日 星期二 02时38分32秒
 ************************************************************************/

#include<iostream>
#include"apue.h"
#include<netinet/in.h>
#include<netdb.h>
#include<sys/socket.h>
using namespace std;

void print_family(struct addrinfo* aip)
{
    cout << "family" << endl;
    switch(aip->ai_family)
    {
        case AF_INET:
            cout << "inet" << endl;
            break;
        case AF_INET6:
            cout << "inet6" << endl;
            break;
        case AF_UNIX:
            cout << "unix" << endl;
            break;
        case AF_UNSPEC:
            cout << "unspecified" << endl;
            break;
        default:
            cout << "unknow" << endl;
    }
}

void print_type(struct addrinfo* aip)
{
    cout << "type: " << endl;
    switch(aip->ai_socktype)
    {
        case SOCK_STREAM:
            cout << "stream" << endl;
            break;
        case SOCK_DGRAM:
            cout << "datagram" << endl;
            break;
        case SOCK_SEQPACKET:
            cout << "seqcapket" << endl;
            break;
        case SOCK_RAW:
            cout << "raw" << endl;
            break;
        default:
            cout << "unknow" << enld;
    }
}



































