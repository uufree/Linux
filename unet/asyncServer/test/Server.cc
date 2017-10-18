/*************************************************************************
	> File Name: AsyncTcpServerTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月18日 星期二 17时19分56秒
 ************************************************************************/

#include"../AsyncTcpServer.h"
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>

using namespace cv;

int main(int argc,char** argv)
{
    unet::net::socket::InetAddress serveraddr(6666);
    unet::net::socket::Socket listenfd(unet::net::socket::LISTEN);
    unet::net::socket::bind(listenfd,serveraddr);
    unet::net::socket::listen(listenfd);
    int clientfd = unet::net::socket::accept(listenfd);
    if(clientfd < 0)
        std::cout << "create clientfd error!" << std::endl;
    
    namedWindow("server");
    char* buf = new char[921600];
    while(1)
    {
        unet::file::readn(clientfd,buf,921600);
        Mat image(480,640,CV_8UC3);
        image.data = (uchar*)buf;
        image.reshape(480,640);
        imshow("server",image);
        
        if(char(waitKey(25)) == 'q')
            break;
    }

    return 0;
}

