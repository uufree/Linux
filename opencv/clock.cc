#include <iostream>  
//#include <opencv2/highgui/highgui.hpp>  
//#include <opencv2/imgproc/imgproc_c.h>  
//#include <opencv2/imgproc/imgproc.hpp>  
#include<opencv2/opencv.hpp>

#include <stdio.h>  
#include <time.h>  
#include <sys/timeb.h>  
//#include <sys/time.h>  
//#include<sys\utime.h>  
  
using namespace std;  
using namespace cv;  
  
int main()  
{  
    Mat clk(640, 640, CV_8UC3,Scalar(180,120,50)); //Mat to store clock image  
    Mat back_up(640, 640, CV_8UC3, Scalar(180, 120, 50)); //Mat to store backup image  
  
    Point cent(clk.rows/2, clk.cols/2);  
    Point perim(clk.cols/2, 0);  
    int rad = clk.cols / 2;  
    float sec_angle = 270;  
    float min_angle = 330;  
    float hour_angle = 210;  
  
    vector<Point> pt1,pt2;  
    for (int i = 0; i < 60; i++)  
    {  
        int x1 = cent.x + rad*cos(i * 6 * CV_PI / 180.0);  
        int y1 = cent.y + rad*sin(i * 6 * CV_PI / 180.0);  
        pt1.push_back(Point(x1, y1));  
  
        int x2 = cent.x + (rad - 20)*cos(i * 6 * CV_PI / 180.0);  
        int y2 = cent.y + (rad - 20)*sin(i * 6 * CV_PI / 180.0);  
        pt2.push_back(Point(x2, y2));  
  
        line(clk, pt1[i], pt2[i], Scalar(0, 255, 0, 0), 1.5, CV_AA, 0);  
    }  

    vector<Point> pt3,pt4;  
    for (int i = 0; i < 12; i++)  
    {  
        int x3 = cent.x + (rad - 40)*cos(i * 30 * CV_PI / 180.0);  
        int y3 = cent.y + (rad - 40)*sin(i * 30 * CV_PI / 180.0);  
        pt3.push_back(Point(x3, y3));  
  
        line(clk, pt1[(i*5)], pt3[i], Scalar(0, 255, 0, 0), 5, CV_AA, 0);  
    }  
    circle(clk, cent, rad, Scalar(50, 50, 255, 0), 6, CV_AA, 0); //Dreaw outercircle of clock  
    circle(clk, cent, 2, Scalar(0, 255, 0, 0), 5, CV_AA, 0); //Draw inner circle  
  
    back_up = clk.clone(); // Clone to backup image  
  
    time_t rawtime;  
    struct tm * timeinfo;  
    float second;  
    float minute;  
    float hour;  
    float millisec;  
    struct timeb tmb;  
  
  
    while (1){  
        //获取本地时间  
        ftime(&tmb);  
        rawtime = tmb.time;  
        timeinfo = localtime (&rawtime);  
  
        second = timeinfo->tm_sec;  
        minute = timeinfo->tm_min;  
        hour = timeinfo->tm_hour;  
        millisec = tmb.millitm;  
  
  
        second = second + millisec / 1000;  
        sec_angle = (second * 6) + 270; //Convert second to angle  
  
        minute = minute + second / 60;  
        min_angle = minute * 6 + 270; //Conver minute to angle  
  
        if (hour>12)hour = hour - 12;  
        hour_angle = (hour * 30) + (minute*.5) + 270; //Conver hour to angle  
  
  
        if (sec_angle>360)sec_angle = sec_angle - 360;  
        if (min_angle>360)min_angle = min_angle - 360;  
        if (hour_angle>360)hour_angle = hour_angle - 360;  
  
        //画秒针  
        perim.x = (int)round(cent.x + (rad - 5) * cos(sec_angle * CV_PI / 180.0));  
        perim.y = (int)round(cent.y + (rad - 5) * sin(sec_angle * CV_PI / 180.0));  
        line(clk, cent, perim, Scalar(0, 255, 255, 0), 1.5, CV_AA, 0);  
  
  
        //画分针  
        perim.x = (int)round(cent.x + (rad - 30) * cos(min_angle * CV_PI / 180.0));  
        perim.y = (int)round(cent.y + (rad - 30) * sin(min_angle * CV_PI / 180.0));  
        line(clk, cent, perim, Scalar(0, 255, 255, 0), 4, CV_AA, 0);  
  
  
        //画时针  
        perim.x = (int)round(cent.x + (rad - 100) * cos(hour_angle * CV_PI / 180.0));  
        perim.y = (int)round(cent.y + (rad - 100) * sin(hour_angle * CV_PI / 180.0));  
        line(clk, cent, perim, Scalar(0, 255, 255, 0), 12, CV_AA, 0);  
  
  
        imshow("Clock", clk); //Show result in a window  
        clk.setTo(0); // set clk image to zero for next drawing  
        clk = back_up.clone(); // Clone the previously drawned markings from back-up image  
  
        char c = waitKey(999); // 这里如果参数为10，则看到的是秒针连续地转动；如果是1000，则效果是秒针一秒一秒地跳动  
        if (c == 27)break;  
    }  
  
    return 0;  
}  
