/*************************************************************************
	> File Name: HandleEvent.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年10月20日 星期五 01时31分45秒
 ************************************************************************/

#include<iostream>

static int mode = 1;
static int runing = false;

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

