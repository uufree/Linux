/*************************************************************************
	> File Name: TcpConnection.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月12日 星期日 09时50分36秒
 ************************************************************************/

#include"TcpConnection.h"
#include<iostream>

namespace unet
{
    namespace net
    {
        TcpConnection::TcpConnection(int fd_) : 
            confd(fd_),
            outputBuffer(fd_),
            inputBuffer(fd_)
        {};

        TcpConnection::TcpConnection(TcpConnection&& lhs):
            confd(std::move(lhs.confd)),
            outputBuffer(std::move(lhs.outputBuffer)),
            inputBuffer(std::move(lhs.inputBuffer)),
            readCallBack(std::move(lhs.readCallBack)),
            writeCallBack(std::move(lhs.writeCallBack)),
            closeCallBack(std::move(lhs.closeCallBack))
        {};

        TcpConnection& TcpConnection::operator=(TcpConnection&& lhs)
        {
            confd = std::move(lhs.confd);
            outputBuffer = std::move(lhs.outputBuffer);
            inputBuffer = std::move(lhs.inputBuffer);
            readCallBack = std::move(lhs.readCallBack);
            writeCallBack = std::move(lhs.writeCallBack);
            closeCallBack = std::move(lhs.closeCallBack);

            return *this;
        }

        TcpConnection::~TcpConnection()
        {};

        int TcpConnection::read()
        {
            int n = inputBuffer.readInSocket();
            if(n <= 0)
            {
                closeCallBack(confd.getFd());
            }
            
            return n;
        }

        void TcpConnection::handleRead()
        {//处理读事件   
            if(readCallBack)
            {
                readCallBack(&inputBuffer,&outputBuffer);
            }
            else
                perror("没有注册readcallback\n");
        }

        void TcpConnection::handleWrite()
        {//处理写事件
            if(writeCallBack)
                writeCallBack(&inputBuffer,&outputBuffer);
            else
                perror("没有注册writecallback");
        }

        void TcpConnection::handleClose()
        {   
            if(closeCallBack)
                closeCallBack(confd.getFd());
            else
                perror("没有注册handlediedtcpconnection\n");

        }
    }
}


