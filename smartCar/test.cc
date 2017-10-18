/*************************************************************************
	> File Name: test.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年10月18日 星期三 19时25分16秒
 ************************************************************************/

#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>

using namespace cv;

int main(int argc,char** argv)
{
    Mat img(imread("/home/uuchen/uuchen.jpeg",CV_LOAD_IMAGE_COLOR));
    namedWindow("home");
    int key;
    
    while(1)
    {
        imshow("home",img);   
        
        if(char(waitKey(30)) == 'q')
        { 
            std::cout << "recv a q!" << std::endl;
            break;
        }             
    }
    return 0;
}

