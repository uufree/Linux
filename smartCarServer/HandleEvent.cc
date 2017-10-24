/*************************************************************************
	> File Name: HandleEvent.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年10月20日 星期五 01时31分45秒
 ************************************************************************/

#include<iostream>
#include<wiringPi.h>

static int mode = 1;
static int runing = false;

static const int PWM1 = 0;
static const int PWM2 = 1;
static const int LEFT1 = 2;
static const int LEFT2 = 3;
static const int RIGHT1 = 4;
static const int RIGHT2 = 5;
static int value = 512;

void handleEvent(char ch)
{
    if(mode == 1)
    {//控制模式
        switch (ch)
        {
            case 'w':
                std::cout << "响应前进～" << std::endl;
                break;
            case 's':
                std::cout << "响应后退～" << std::endl;
                break;
            case 'a':
                std::cout << "响应左转～" << std::endl;
                break;
            case 'd':
                std::cout << "响应右转～" << std::endl;
                break;
            case char(32):
            {
                if(runing)
                {
                    std::cout << "响应停止～" << std::endl;
                    runing = false;
                }
                else
                {
                    std::cout << "响应开始～" << std::endl;
                    runing = true;
                }
                break;
            }
            case char(50):
                std::cout << "切换到寻迹模式～" << std::endl;
                mode = 2;
                break;
            case char(51):
                std::cout << "切换到避障模式～" << std::endl;
                mode = 3;
                break;
            default:
                break;
        }
    }
    else if(mode == 2)
    {//寻迹模式
        switch (ch)
        {
            case char(32):
            {
                if(runing)
                {
                    std::cout << "响应停止～" << std::endl;
                    runing = false;
                }
                else
                {
                    std::cout << "响应开始～" << std::endl;
                    runing = true;
                }
                break;
            }
            case char(49):
                std::cout << "切换到控制模式～" << std::endl;
                mode = 1;
                break;
            case char(51):
                std::cout << "切换到避障模式～" << std::endl;
                mode = 3;
                break;
            default:
                break;
        }
    }
    else if(mode == 3)
    {//避障模式
        switch (ch)
        {
            case char(32):
            {
                if(runing)
                {
                    std::cout << "响应停止～" << std::endl;
                    runing = false;
                }
                else
                {
                    std::cout << "响应开始～" << std::endl;
                    runing = true;
                }
                
                break;
            }
            case char(49):
                std::cout << "切换到控制模式～" << std::endl;
                mode = 1;
                break;
            case char(50):
                std::cout << "切换到寻迹模式～" << std::endl;
                mode = 2;
                break;
            default:
                break;
        }
    }
};

void setup()
{
    if(wiringPiSetup())
    {
        std::cerr << "setup error!" << std::endl;
        return -1;
    }

    pinMode(PWM1,PWM_OUTPUT);
    pinMode(PWM2,PWM_OUTPUT);
    pinMode(LEFT1,OUTPUT);
    pinMode(LEFT2,OUTPUT);
    pinMode(RIGHT1,OUTPUT);
    pinMode(RIGHT2,OUTPUT);
    
    digitalWrite(PWM1,LOW);
    digitalWrite(PWM2,LOW);
    digitalWrite(LEFT1,LOW);
    digitalWrite(LEFT2,LOW);
    digitalWrite(RIGHT1,LOW);
    digitalWrite(RIGHT2,LOW);
}

void forward()
{
     
}
