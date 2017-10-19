/*************************************************************************
	> File Name: AsyncTcpServerTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月18日 星期二 17时19分56秒
 ************************************************************************/

#include"InetAddress.h"
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>

using namespace cv;

int main(int argc,char** argv)
{
    VideoCapture capture(0);
    if(!capture.isOpened())
    {
        std::cout << "摄像头没有打开!" << std::endl;
        return -1;
    }

    unet::net::socket::InetAddress serveraddr(6666);
    unet::net::socket::Socket listenfd(unet::net::socket::LISTEN);
    unet::net::socket::bind(listenfd,serveraddr);
    unet::net::socket::listen(listenfd);
    
    Mat image;
    while(1)
    {
        int clientfd = unet::net::socket::accept(listenfd);
        if(clientfd < 0)
            std::cout << "create clientfd error!" << std::endl;
        
        while(1)
        {
            capture >> image;
            image = image.reshape(0,480);
            std::string str((char*)image.data,921600);     
            unet::file::writen(clientfd,str.c_str(),921600); 

            waitKey(20);
        }
    }
    
    return 0;
}









