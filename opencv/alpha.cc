/*************************************************************************
	> File Name: alpha.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月02日 星期五 19时18分50秒
 ************************************************************************/

#include<iostream>
#include<opencv2/opencv.hpp>

int main(int argc,char** argv)
{
    cv::Mat uuchen = cv::imread("/home/uuchen/uuchen.jpeg");
    cv::Mat chenuu = cv::Mat::zeros(uuchen.size(),uuchen.type());

    uuchen.convertTo(chenuu,-1,0.1,100);

    cv::imshow("chenuu",chenuu);
    cv::waitKey();
    
    return 0;
}

