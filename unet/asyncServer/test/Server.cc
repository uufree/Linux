/*************************************************************************
	> File Name: AsyncTcpServerTest.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月18日 星期二 17时19分56秒
 ************************************************************************/

#include"../AsyncTcpServer.h"
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
/*
void readCallBack(unet::net::Buffer* inputBuffer,unet::net::Buffer* outputBuffer)
{
    std::string message;
    inputBuffer->getCompleteMessageInBuffer(message);

    std::cout << "readCallBack: " << message << std::endl;
}

void timeReadCallBack()
{
    std::cout << "Timer was locked!" << std::endl;
}
*/

int main(int argc,char** argv)
{
    unet::net::socket::InetAddress serveraddr(6666);
    unet::net::AsyncTcpServer server(serveraddr,1);
    server.setReadCallBack(std::bind(&readCallBack,std::placeholders::_1,std::placeholders::_2));

//    unet::time::TimerPtr timer(new unet::time::Timer(true,1));
//    timer->setTimeCallBack(std::bind(&timeReadCallBack));
//    server.addTimer(std::move(timer));
    
    server.start();

    return 0;
}

