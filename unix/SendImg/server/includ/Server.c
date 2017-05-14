/*************************************************************************
	> File Name: Server.c
	> Author: 
	> Mail: 
	> Created Time: 2016年10月23日 星期日 20时07分34秒
 ************************************************************************/

#include"Server.h"
#include"Public.h"
#include"Public.c"


int BuildListenSocket()
{//创建监听套接字
    struct sockaddr_in serveraddr;
    int listenfd;
    while(1)
    {
        if((listenfd=socket(AF_INET,SOCK_STREAM,0)) < 0)
            continue;

        bzero(&serveraddr,sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;    
        serveraddr.sin_addr.s_addr = htonl(serveraddr.sin_addr.s_addr);
        serveraddr.sin_port = htons(SERV_PORT);

        if(bind(listenfd,(sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
        {
            close(listenfd);
            continue;
        }

        if(listen(listenfd,LISTEN) == 0)
            break;
        else
        {
            close(listenfd);
            continue;
        }
    }

    return listenfd;
}

int BuildConnetSocket(const int& listenfd)//创建链接套接字
{//创建链接套芥子
    int connetfd;
    while(1)
    {
        if((connetfd=accept(listenfd,NULL,NULL)) <= 0)
            continue;
        else
            break;
    }
    
    return connetfd;
}

