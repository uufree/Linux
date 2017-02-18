/*************************************************************************
	> File Name: Server.h
	> Author: 
	> Mail: 
	> Created Time: 2016年10月23日 星期日 19时56分48秒
 ************************************************************************/

#ifndef _SERVER_H
#define _SERVER_H

#include"Public.h"

static struct Node Deque;

int BuildListenSocket();

int BuildConnetSocket(const int& listenfd);

void CleanPthread(void* arg);

static void* ReadImgMessage(void* arg);//读取文件信息线程

static void* SendImg(void* arg);//发送图片函数

static void FreeResource(void);//资源清理函数

static void HandleSignal(int);//处理信号

#endif
