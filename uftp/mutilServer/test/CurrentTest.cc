/*************************************************************************
	> File Name: CurrentTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月28日 星期二 20时30分24秒
 ************************************************************************/

#include"../Current.h"

using namespace unet;
using namespace unet::thread;

int main(int argc,char** argv)
{
    Current current;
    current.startLoop();
    
    return 0;
}

