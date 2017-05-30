/*************************************************************************
	> File Name: test.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年05月30日 星期二 11时23分16秒
 ************************************************************************/

#include"SerialPort.h"
#include<iostream>

using namespace serial;

int main(int argc,char** argv)
{
    SerialPort port("/dev/tty");
    port.printMessage();
    std::cout << "************************" << std::endl;
    SerialPort port1 = port;
    port1.printMessage();
    std::cout << "************************" << std::endl;
    SerialPort port2 = std::move(port);
    port2.printMessage();
    return 0;
}

