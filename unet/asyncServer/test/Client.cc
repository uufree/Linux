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
    net::socket::InetAddress server("192.168.1.106",6666);
    net::socket::Socket confd(net::socket::CONNECT);
    net::socket::connect(confd,server);
    
    namedWindow("client");
    char* buf = new char[921600];
    char ch;
    while(1)
    {
        unet::file::readn(confd.getFd(),buf,921600);
        Mat image(480,640,CV_8UC3);
        image.data = (uchar*)buf;
        image.reshape(480,640);
        imshow("client",image);

        switch (ch = char(waitKey(25)))
        {
            case 'q':
                goto exit;
            case char(27):
                std::cout << "响应ESC～" << std::endl;
                break;
            case char(72):
                std::cout << "响应前进～" << std::endl;
                break;
            case char(80):
                std::cout << "响应后退～" << std::endl;
                break;
            case char(75):
                std::cout << "响应左转～" << std::endl;
                break;
            case char(77):
                std::cout << "响应右转～" << std::endl;
                break;
            default:
                break;
        }
    } 
    
exit:
    return 0;
}

