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

using namespace cv;

int main(int argc,char** argv)
{
    Mat img(900,1600,CV_8UC3,Scalar(0,0,0));
    namedWindow("home",CV_WINDOW_NORMAL);
    setWindowProperty("home",CV_WND_PROP_FULLSCREEN,CV_WINDOW_FULLSCREEN);

    std::vector<std::string> strVec;
    
    strVec.push_back("20:41");
    strVec.push_back("20:42");
    strVec.push_back("20:43");
    strVec.push_back("20:44");
    strVec.push_back("20:45");
    strVec.push_back("20:46");
    strVec.push_back("20:47");
    strVec.push_back("20:48");
    strVec.push_back("20:49");
    strVec.push_back("20:50");

    for(int i=0;i<10;++i)
    {
        putText(img,strVec[i],Point(400,400),FONT_HERSHEY_SIMPLEX,3,Scalar(255,255,255),3);
        imshow("home",img);
        waitKey(999);
        img.setTo(Scalar(0,0,0));
    }
    return 0;
}
