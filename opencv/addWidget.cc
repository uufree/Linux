/*************************************************************************
	> File Name: addWidget.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月02日 星期五 18时48分45秒
 ************************************************************************/

#include<iostream>
#include<opencv2/opencv.hpp>

int main(int argc,char** argv)
{
    cv::Mat src1 = cv::imread("/home/uuchen/1.jpg");
    if(src1.empty())
    {
        std::cerr << "Open uuchen.jpeg error" << std::endl;
        return -1;
    }

    cv::Mat src2 = cv::imread("/home/uuchen/2.jpg");
    if(src2.empty())
    {
        std::cerr << "Open chenuu.jpg error" << std::endl;
        return -1;
    }
    
    cv::Mat result;
    cv::addWeighted(src1,0.5,src2,0.5,0,result);

    imshow("result",result);
    cv::waitKey();

    return 0;
}
