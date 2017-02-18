/*************************************************************************
	> File Name: Public.c
	> Author: 
	> Mail: 
	> Created Time: 2016年10月23日 星期日 20时03分51秒
 ************************************************************************/

#include"Public.h"

int Readn(int fd,void* ptr,int n)//封装系统函数
{
    int nleft;
    int nread;
    char* cptr;

    cptr = (char*)ptr;
    nleft = n;
    while(nleft > 0)
    {
        if((nread=read(fd,cptr,nleft)) < 0)
        {
            if(errno == EINTR)
                nread = 0; 
            else
                return -1; 
        }
        else if(nread == 0)
            break;  
        nleft -= nread;
        cptr += nread;
    }

    return (n-nleft);
}

int Writen(int fd,void* vptr,int n)
{
    int nleft;
    int nwritten;
    char* ptr;

    ptr = (char*)vptr;
    nleft = n;
    while(nleft>0)  
    {
        if((nwritten=write(fd,ptr,nleft)) <= 0)
        {
            if(nwritten<0 && errno!=EINTR)
                nwritten = 0;    
            else
                return -1;  
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n-nleft);
}

int SetNonBlocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_GETFL,new_option);
    return 0;
}

int BuildEpoll(const int& fd,int& epollfd,epoll_event& event)
{//
    epollfd = epoll_create(1);
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    return 1;
}
    
/*int judge_epoll(const int& fd,int& epollfd,epoll_event& event)
{   
    epoll_event events;
    int ret = epoll_wait(epollfd,&events,1,-1);

    if(ret < 0)
        return -1;
    else
        return 1;
}*/

int ResetEpoll(const int& fd,int& epollfd,epoll_event& event)
{
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
    return 1;
}
    
int JudgeEpoll(const int& fd)
{
    fd_set sel;
    FD_ZERO(&sel);
    FD_SET(fd,&sel);
    select(fd+1,&sel,NULL,NULL,NULL);
    if(FD_ISSET(fd,&sel))
        return 1;
    else
        return -1;
}





























