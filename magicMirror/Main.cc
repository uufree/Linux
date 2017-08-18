/*************************************************************************
	> File Name: circle.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月02日 星期五 20时03分04秒
 ************************************************************************/

#include<iostream>
#include<vector>
#include<utility>
#include<unistd.h>
#include<opencv2/opencv.hpp>
#include"GuiResources.h"

using namespace cv;

int main(int argc,char** argv)
{
    Mat img(900,1600,CV_8UC3,Scalar(0,0,0));
    namedWindow("home",CV_WINDOW_NORMAL);
    setWindowProperty("home",CV_WND_PROP_FULLSCREEN,CV_WINDOW_FULLSCREEN);
    
    GuiResources resources; 

    while(1)
    {
        putText(img,resources.getClock_MouthAndDays(),Point(100,700),FONT_HERSHEY_SIMPLEX,3,Scalar(255,255,255),3);
        putText(img,resources.getClock_HourAndMinutes(),Point(100,800),FONT_HERSHEY_SIMPLEX,2,Scalar(255,255,255),3);
        imshow("home",img);
        waitKey(999);
        img.setTo(Scalar(0,0,0));
    }

    return 0;
}
