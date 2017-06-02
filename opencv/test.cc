/*************************************************************************
	> File Name: test.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月02日 星期五 11时56分09秒
 ************************************************************************/

#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;

int main(int argc,char** argv)
{
    Mat image = imread("/home/uuchen/uuchen.jpeg",1);

    if(!image.data)
    {
        std::cerr << "no image data!" << std::endl;
        return -1;
    }
    namedWindow("Display Image",WINDOW_AUTOSIZE);
    imshow("Display Image",image);

    waitKey(0);

    return 0;
}

