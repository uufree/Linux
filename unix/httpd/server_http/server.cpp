/*********************************************************************
	> File Name: server.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年10月13日 星期四 20时23分20秒
 ********************************************************************/
#include"server.h"
using namespace std;

/***************socket包裹函数*****************/

ssize_t readn(int fd,void* ptr,size_t n)//read包裹函数
{
    size_t nleft;
    ssize_t nread;
    char* cptr;

    cptr = (char*)ptr;
    nleft = n;
    while(nleft > 0)
    {
        if((nread=read(fd,cptr,nleft)) < 0)
        {
            if(errno == EINTR)
                nread = 0; //call again
            else
            {
                cerr << "readn error" << endl;
                return -1; //error
            }
        }
        else if(nread == 0)
            break;  //EOF
        nleft -= nread;
        cptr += nread;
    }

    return (n-nleft);
}

ssize_t writen(int fd,void* vptr,size_t n)//write包裹函数
{
    size_t nleft;
    ssize_t nwritten;
    char* ptr;

    ptr = (char*)vptr;
    nleft = n;
    while(nleft>0)  
    {
        if((nwritten=write(fd,ptr,nleft)) <= 0)
        {
            if(nwritten<0 && errno!=EINTR)
                nwritten = 0;    //call again
            else
            {
                cerr << "writen error" << endl;
                return -1;  //error
            }
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

int setnonblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_GETFL,new_option);
    return 0;
}

/**********************httpwaitlock方法*************************/
int httpwaitlock::judge_stadeque() const
{
    pthread_rwlock_rdlock(&httplock);
    if(!static_httpwait.empty())
    {
        pthread_rwlock_unlock(&httplock);
        return 0;
    }
    else
    {
        pthread_rwlock_unlock(&httplock);
        return -1;
    }
}

int httpwaitlock::judge_dyndeque() const
{
    pthread_rwlock_rdlock(&httplock);
    if(!static_httpwait.empty())
    {
        pthread_rwlock_unlock(&httplock);
        return 0;
    }
    else
    {
        pthread_rwlock_unlock(&httplock);
        return -1;
    }
}

int httpwaitlock::putstalock(const struct httpwait& stahttpwait)
{
    pthread_rwlock_wrlock(&httplock);
    static_httpwait.push_back(stahttpwait);
    pthread_rwlock_unlock(&httplock);
    return 0;
}

int httpwaitlock::putdynlock(const struct httpwait& dynhttpwait)
{
    pthread_rwlock_wrlock(&httplock);
    dynamic_httpwait.push_back(dynhttpwait);
    pthread_rwlock_unlock(&httplock);
    return 0;
}

httpwait httpwaitlock::getstalock()
{
    pthread_rwlock_rdlock(&httplock);
    pthread_rwlock_wrlock(&httplock);
    httpwait wait = static_httpwait.front();
    static_httpwait.pop_front();
    pthread_rwlock_unlock(&httplock);
    pthread_rwlock_unlock(&httplock);
    return wait;
}

httpwait httpwaitlock::getdynlock()
{
    pthread_rwlock_rdlock(&httplock);
    pthread_rwlock_wrlock(&httplock);
    httpwait wait = dynamic_httpwait.front();
    static_httpwait.pop_front();
    pthread_rwlock_unlock(&httplock);
    pthread_rwlock_unlock(&httplock);
    return wait;
}

/********************fdlock方法************************/
int fdlock::judgefd() const
{
    pthread_rwlock_rdlock(&lock);
    if(!confds.empty())
    {
        pthread_unlock_unlock(&lock);
        return 0;
    }
    else
    {
        pthread_unlock_unlock(&lock);
        return -1;
    }
}

int fdlock::putfd(int fd);
{
    pthread_rwlock_wrlock(&lock);
    confds.push_back(fd);
    pthread_rwlock_unlock(&lock);
    return 0;
}

int fdlock::getfd()
{
    pthread_rwlock_wrlock(&lock);
    int fd = confds.front();
    confds.pop.front();
    pthread_rwlock_unlock(&lock);
    return fd;
}

/*****************server类函数***************************/
int Server::Communication() const
{
    struct socketaddr_in serveraddr;
    listenfd = socket(AF_INET,SOCK_STREAM,0);

    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(serveraddr.sin_addr.s_addr);
    server.sinport = htons(SERV_PORT);

    bind(listenfd,(sockaddr*)&serveraddr,sizeof(serveraddr));
    listen(listenfd,LISTEN);
}

int Server::judge_listenfd_read() const
{
    fd_set sel;
    FD_ZERO(&sel);
    FD_SET(listenfd,&sel);
    select(listenfd+1,&sel,NULL,NULL,NULL);
    if(FD_ISSET(listenfd,&sel))
        return 0;
    else
        return -1;
}

struct httpwait Server::analyse(int connetfd) const
{ 
    int n;
    char ch;
    char httpheadline[MAXLINE];
    struct httpwait node;
    
read:
    for(int i=0;i<MAXLINE;i++)//读取http请求首行
    {
        if((n=read(connetfd,&ch,1)) == 1)
        {
            httpheadline[i] = ch;
            if(ch == '\n')//读完
                break;
        }
        else if(n == 0)
        {
            if(i == 0)
                return -1;//没有读到数据
            else
                break;//读完
        }
        else
        {
            if(errno != EINTR)//是否被中断
                return -1;
            else
                goto read;//重新读
        }
    }

    string line = httpheadline;
    string iter;
    stringstream ss(line);
    ss >> iter;
    strcpy(node.requesttype,iter.c_str());
    ss >> iter;
    node.httpurl = iter;
    ss >> iter;
    strcpy(node.httpmessage,iter.c_str());
    iter = node.httpurl;

    if(!iter.find('?'))//静态解析
    {
        for(int i=0;i<iter.size();++i)
        {
            if(iter[i]=='/' && iter[i+1]!='/' && iter[i-1]!='/')
            {
                string kl(iter,i);
                node.httpurl = kl;
                node.urltype = 1;
                break;
            }
        }
        return node;
    }
    else//动态解析
    {
        string urli = "";
        int i = 0;
        int size = iter.size();
        for(;i<size;++i)//地址解析
        {
            if(iter[i]=='/' && iter[i-1]!='/' && iter[i+1]!='/')
            {
                while(iter[i] != '?')
                {
                    node.httpurl += iter[i];
                    ++i;
                }
                ++i;
                break;
            }
        }

        for(;i<size;++i)//参数解析
        {
            if(iter[i] != '&')
            {
                urli += iter[i];
                continue;
            }
            else
            {
                node.dynamicstr.push_back(urli);
                addr = "";
            }
        }
        node.dynamicstr.push_back(urli);
        return node;
    }
}

int httpwait

        
        

    
    

    





    



















