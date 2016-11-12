/*************************************************************************
	> File Name: Client.c
	> Author: 
	> Mail: 
	> Created Time: 2016年10月26日 星期三 17时49分49秒
 ************************************************************************/

#include"Public.h"
#include"Public.c"
#include"Client.h"

int n = 1;

int CreateConnet()//创建链接套芥子
{
    struct sockaddr_in serveraddr;
    int socketfd;
    while(1)
    {
        if((socketfd=socket(AF_INET,SOCK_STREAM,0)) < 0)
            continue;
    
        bzero(&serveraddr,sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(SERV_PORT);
        inet_pton(AF_INET,"127.0.0.1",&serveraddr.sin_addr);

        if(connect(socketfd,(sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
            continue;
        else
            break;
    }
    return socketfd;
}

int RecvImgMessage(const int& connetfd)
{//接受信息
    int num;
    int imgfd;
    char buf[SIZE];
    struct Packet phs[1];

    while(1)//数据包的解析函数
    {//具体逻辑不太好形容，反正写的很懒
        memset(phs,'\0',PACKETSIZE);
        num = recv(connetfd,phs,PACKETSIZE,0);
        if(num > 0)
        {
            if(strcmp(buf,phs->filename) != 0)
            {
                memset(buf,'\0',SIZE);
                strcpy(buf,phs->filename);
                imgfd = CreateImgFd(phs->filename);
                CreateMegFile(phs);
                Writen(imgfd,phs->data,DATASIZE);
                while(1)
                {
                    num = recv(connetfd,phs,PACKETSIZE,0);
                    if(num > 0)
                    {
                        if(strcmp(buf,phs->filename) == 0)
                            Writen(imgfd,phs->data,DATASIZE);
                        else
                        {
                            close(imgfd);
                            strcpy(buf,phs->filename);
                            imgfd = CreateImgFd(phs->filename);
                            CreateMegFile(phs);
                            Writen(imgfd,phs->data,DATASIZE);
                        }
                    }
                    else if(num == 0)
                    {
                        close(imgfd);
                        return -1;
                    }
                    else
                    {
                        close(imgfd);
                        perror("recv error!\n");
                        exit(0);
                    }
                }
            }
        }
        else
            exit(0);
    }

    return 1;
}

int CreateDirent()
{//在客户端文件夹创建保存图片的文件夹
    char img[SIZE];
    char meg[SIZE];

    memset(img,'\0',SIZE);
    memset(meg,'\0',SIZE);
    strcpy(img,"/home/will/recv_img/");//可以手动修改
    strcpy(meg,"/home/will/recv_meg/");

    if(mkdir(img,S_IRWXU) < 0)
        return -1;
    if(mkdir(meg,S_IRWXU) < 0)
        return -1;
    return 1;
}

int CreateImgFd(const char* pos)//创建图片文件的Fd
{
    int imgfd;
    char img[128];

    memset(img,'\0',128);
    strcpy(img,"/home/will/recv_img/");
    strcat(img,pos);

    printf("%d%s\n",n,img);
    ++n;

    if((imgfd=open(img,O_CREAT|O_WRONLY|O_APPEND,S_IRWXU)) < 0)
    {
        printf("imgfd: %d\n",imgfd);
        exit(0);
    }

    return imgfd;

}

int CreateMegFile(Packet* phs)//创建图片信息的文件
{

    int megfd;
    char meg[128];
    char n[] = "\n";

    memset(meg,'\0',128);
    strcpy(meg,"/home/will/recv_meg/");
    strcat(meg,phs->filename);
    strcat(meg,".txt");


    while(1)
    {
        if((megfd=open(meg,O_CREAT|O_WRONLY|O_APPEND|O_EXCL,S_IRWXU)) < 0)
            continue;
        else
            break;
    }

    if(megfd > 0)
    {
        memset(meg,'\0',128);
        strcpy(meg,"filename: ");
        Writen(megfd,meg,strlen(meg));
        Writen(megfd,phs->filename,strlen(phs->filename));
        Writen(megfd,n,strlen(n));
        
        memset(meg,'\0',128);
        strcpy(meg,"filesize: ");
        Writen(megfd,meg,strlen(meg));
        memset(meg,'\0',128);
        sprintf(meg,"%d",phs->filesize);
        Writen(megfd,meg,strlen(meg));
    }
    
    close(megfd);
    return 1;
}








