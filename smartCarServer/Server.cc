/*************************************************************************
	> File Name: AsyncTcpServerTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月18日 星期二 17时19分56秒
 ************************************************************************/

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
#include<iostream>
#include<string>

#include"File.h"
#include"Thread.h"
#include"InetAddress.h"
#include"Socket.h"
#include"Mutex.h"

using namespace cv;

void handleEvent(char ch);

static int confd;
static bool isConnected = false;

void threadFunc()
{
    char ch;
    while(1)
    {
        if(isConnected)
        {
            int n = unet::file::readn(confd,&ch,1);
            if(n == 0)
            {
                std::cout << "client->confd is close!" << std::endl;
                return;
            }
            handleEvent(ch);
        }
    }
}

int main(int argc,char** argv)
{
/*
    VideoCapture capture(0);
    if(!capture.isOpened())
    {
        std::cout << "摄像头没有打开!" << std::endl;
        return -1;
    }
*/    
    unet::net::socket::InetAddress serveraddr(16666);
    unet::net::socket::Socket listenfd(unet::net::socket::LISTEN);
    unet::net::socket::bind(listenfd,serveraddr);
    unet::net::socket::listen(listenfd);
    confd = unet::net::socket::accept(listenfd.getFd());
    isConnected = true;
   
    char ch;
    while(1)
    {
        if(isConnected)
        {
            int n = unet::file::readn(confd,&ch,1);
            if(n == 0)
            {
                std::cout << "client->confd is close!" << std::endl;
                return 0;
            }
            handleEvent(ch);
        }
    }
/*
    unet::thread::Thread thread;
    thread.setThreadCallBack(std::bind(&threadFunc));
    thread.start();
    
    Mat image;
    while(isConnected)
    {
        capture >> image;
        image = image.reshape(0,480);
        std::string str((char*)image.data,921600);     
        unet::file::writen(confd,str.c_str(),921600); 
        waitKey(20);
    }
*/    
    return 0;
}


