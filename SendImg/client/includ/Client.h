/*************************************************************************
	> File Name: Client.h
	> Author: 
	> Mail: 
	> Created Time: 2016年10月23日 星期日 20时01分17秒
 ************************************************************************/

#ifndef _CLIENT_H
#define _CLIENT_H

#include"Public.h"

int CreateConnet();//创建链接套接字

int RecvImgMessage(const int& connetfd);//读取图片信息

int CreateDirent();//创建存放图片的文件夹

int CreateImgFd(const char* pos);//创建文件的fd

int CreateMegFile(Packet* phs);//创建图片信息的文件

static void FreeResource(void);//资源处理函数

static void HandleSignal(int);//信号处理函数

#endif
