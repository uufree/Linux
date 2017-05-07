/**********************************************************************
	> File Name: Main_Server.c
	> Author:UUChen
	> Mail: 
	> Created Time: 2016年10月28日 星期五 16时15分40秒
 *********************************************************************/

#include"Public.h"
#include"Server.h"
#include"Server.c"

int listenfd;//监听套接字,发送图片线程，把这两个放外面实属无奈～～
pthread_t tid;
int size = 1;

int main(int argc,char** argv)
{
    struct epoll_event event;//阻塞变量
    int epollfd,connetfd;
    char buf[SIZE];
    int beforeconnetfd;
    memset(buf,'\0',SIZE);
    pthread_t pid;//线程资源

    if(atexit(FreeResource) < 0)//注册exit
    {
        perror("atexit error\n");
        exit(0);
    }
    
    Deque.Lock = PTHREAD_MUTEX_INITIALIZER;//初始队列信息
    Deque.head = (File*)malloc(sizeof(File));
    Deque.tail = Deque.head;
    
    if((listenfd=BuildListenSocket()) < 0)//创建监听套接字
    {
        printf("BuildListenSocket error\n");
        exit(0);
    }
    printf("listenfd: %d\n",listenfd);


    if(signal(SIGTSTP,HandleSignal) == SIG_ERR)//信号处理函数
    {
        perror("signal error!\n");
        exit(0);
    }

    if(signal(SIGUSR1,HandleSignal) == SIG_ERR)
    {
        perror("signal error!\n");
        exit(0);
    }

    if(signal(SIGINT,HandleSignal) == SIG_ERR)
    {
        perror("signal error!\n");
        exit(0);
    }

    int times = 1;
    while(JudgeEpoll(listenfd))//判断监听套接字是否可读
    {
        printf("times: %d\n",times);
        if((connetfd=BuildConnetSocket(listenfd)) < 0)
        {//创建连接套接字
            perror("BuildConnetSocket error\n");
            exit(0);
        }
        printf("connetfd = %d\n",connetfd); 
        
        memset(Deque.tail->poet.filename,'\0',SIZE);
        memset(Deque.tail->poet.data,'\0',DATASIZE);

        if(pthread_create(&pid,NULL,ReadImgMessage,(void*)&connetfd) < 0)
        {//启动图片读取线程
            perror("pthraed_create error\n");
            exit(0);
        }
        
        if(times == 1)
        {
            if(pthread_create(&tid,NULL,SendImg,NULL) < 0)
            {//启动发送文件线程
                perror("pthread_create error\n");
                exit(0);
            }
        }

        printf("/********************************************/\n");
        pthread_join(pid,NULL);//捕捉线程结束信号
        if(times == 1)
            beforeconnetfd = connetfd;
        else
        {
            close(beforeconnetfd);
            beforeconnetfd = connetfd;   
        }
        ++times;
	printf("Server Send Finish!\n");
        printf("/********************************************/\n");
    }

    exit(0);
}
    
static void* ReadImgMessage(void* arg)//文件读取线程
{
    int connetfd = (*(int*)arg);
    char buf[SIZE];
    DIR* dp;
    struct dirent* dirp;
    struct stat sta;
    int times;

    memset(buf,'\0',SIZE);
    strcpy(buf,"/home/will/send/");//打开图片所在文件夹，可修改
    if((dp=opendir(buf)) == NULL)
    {
        perror("thread opendir error!\n");
        exit(0);
    }

    while((dirp=readdir(dp)) != NULL)//循环读取文件中所有的图片
    {
        if((strcmp(dirp->d_name,".") == 0) || (strcmp(dirp->d_name,"..") == 0))//过滤掉.和..
            continue;
         
        strcat(buf,dirp->d_name);
//        printf("%d%s\n",size,buf);//在终端输出发送信息
        ++size;
        if(stat(buf,&sta) < 0)
        {
            perror("get file message error\n");
            exit(0);
        }
        
        int fd = open(buf,O_RDONLY);

        while(1)//此线程在head和tail指向听一块位置时，创建一个新的节点，将tail移动到新的节点，并将信息读到新的节点
        {//然后等待下一次tail和head指向同一个节点
            if(Deque.tail == Deque.head)
            {
                pthread_mutex_lock(&Deque.Lock);//必须加大锁的临界区
                Deque.tail->next = (File*)malloc(sizeof(File));
                Deque.tail = Deque.tail->next;
                Deque.tail->next = NULL;
                Deque.head->connetfd = connetfd;
                memset(Deque.head->poet.filename,'\0',SIZE);
                strcpy(Deque.head->poet.filename,dirp->d_name);
                Deque.head->poet.filesize = sta.st_size;

                memset(Deque.head->poet.data,'\0',DATASIZE);
                times = Readn(fd,Deque.head->poet.data,DATASIZE);
                pthread_mutex_unlock(&Deque.Lock); 
                if(times == DATASIZE)//一个图片没读完，继续读   
                    continue;
                else if(times > 0 && times < DATASIZE)//读完一张图片，Break出去获得新图片的文件按描述符
                    break;
                else
                {
                    close(fd);
                    perror("writen error!\n");
                    exit(0);
                }
            }
            else
                continue;
        }

        close(fd);
        memset(buf,'\0',SIZE);
        strcpy(buf,"/home/will/send/");
    }
    pthread_exit(0);    
}

static void* SendImg(void* arg)//文件发送线程
{
    pthread_detach(pthread_self());//设置为分离属性
    char buffer[SIZE];
    char buf[SIZE];
    
    while(1)//如果head->next指向tail，发送Head的节点，然后将head移动到tail指向的节点上
    {
        if(Deque.head->next == Deque.tail)
        {
            if(strcmp(buffer,Deque.head->poet.filename) != 0)
            {
                memset(buffer,'\0',SIZE);
                strcpy(buffer,Deque.head->poet.filename);
            }

            pthread_mutex_lock(&Deque.Lock);//安全大于效率
            File* st = Deque.head;//增加了锁的临界区
            Deque.head = Deque.head->next;
            Deque.head->next = NULL;
                
            if(send(st->connetfd,&st->poet,PACKETSIZE,0) < 0)
            {
                perror("send error!\n");
                exit(0);
            }
            free(st);   
            pthread_mutex_unlock(&Deque.Lock);
        }
        else
            continue;
    }

    pthread_exit(0);
}

static void FreeResource(void)//清理资源函数
{
    close(listenfd);//关闭监听套接字 
    printf("close listenfd!\n");
    pthread_mutex_lock(&Deque.Lock);
    File* st = Deque.head;
    while(Deque.head != NULL)
    {
        Deque.head = Deque.head->next;
        free(st);
        st = Deque.head;
    }
    pthread_mutex_unlock(&Deque.Lock);
}

static void HandleSignal(int signo)//处理信号函数
{
    if(signo == SIGTSTP)
        exit(0);
        
    if(signo == SIGUSR1)
    {
/*        pthread_cancel(pid);
        deque.head = deque.reserve;
        send_ = 0;
        if(pthread_create(&pid,NULL,send_imging,(void*)&connetfd) < 0)//线程没能重复启动成功
        {//启动发送文件线程
            perror("pthread_create error\n");
            exit(0);
        }  */
        printf("receive SIGUSR1!\n");
    }

    if(signo == SIGINT)
        exit(0);
}





    
    
































