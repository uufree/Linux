/*************************************************************************
	> File Name: Main_Client.c
	> Author: 
	> Mail: 
	> Created Time: 2016年10月28日 星期五 16时16分56秒
 ************************************************************************/


#include"Public.h"
#include"Client.h"
#include"Client.c"

int connetfd;

int times = 1;

int main(int argc,char** argv)
{
    int epollfd,connetfd;//阻塞变量
    struct epoll_event event;

    if(atexit(FreeResource) < 0)//注册atexit函数
    {
        perror("atexit error");
        exit(0);
    }

    CreateDirent();//创建存放图片和图片信息的文件夹

    if(signal(SIGTSTP,HandleSignal) == SIG_ERR)//注册信号测试函数
    {
        perror("signal error!\n");
        exit(0);
    }

    printf("times: %d\n",times);
    ++times;
    if((connetfd=CreateConnet()) < 0)//创建连接套接字
    {
        perror("create connetfd error\n");
        exit(0);
    }
    printf("connetfd: %d\n",connetfd);

    if(JudgeEpoll(connetfd))//判断链接套接字是否可读
    {   
        while(1)
        {
            if(RecvImgMessage(connetfd) < 0)//处理接受的信息
            {//接受信息和图片
                perror("recv finished!\n");
                break;
            }
        }
        printf("/*********************************************/\n");
    }

    exit(0);
}

static void FreeResource(void)//关闭链接套接字，不合理在于全局变量
{
    close(connetfd);
    printf("connetfd already close!\n");
    printf("Client finished!\n");
}

static void HandleSignal(int signo)//处理信号函数
{
    if(signo == SIGTSTP)
        exit(0);
}
