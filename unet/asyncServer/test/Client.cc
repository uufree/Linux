/*************************************************************************
	> File Name: Client.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月15日 星期六 17时12分46秒
 ************************************************************************/

#include"../../Buffer.h"
#include"../../Socket.h"
#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
#include<string>

using namespace unet;
using namespace cv;

int main(int argc,char** argv)
{
    VideoCapture capture(0);
    if(!capture.isOpened())
    {
        std::cout << "摄像头没有打开!" << std::endl;
        return -1;
    }
    
    net::socket::InetAddress server("192.168.1.106",6666);
    net::socket::Socket confd(net::socket::CONNECT);
    net::socket::connect(confd,server);
    
    Mat image;
    namedWindow("client");
    while(1)
    {
        capture >> image;
        image = image.reshape(0,480);
        std::string str((char*)image.data,921600);     
        imshow("client",image);
        unet::file::writen(confd.getFd(),str.c_str(),921600); 

        if(char(waitKey(25)) == 'q')
            break;
    }
    
    return 0;
}










