/*************************************************************************
	> File Name: TcpConnection.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月12日 星期日 09时50分36秒
 ************************************************************************/

#include"TcpConnection.h"

namespace unet
{
    namespace net
    {
        TcpConnection::TcpConnection(int fd_) : 
            confd(fd_),
            outputbuffer(fd_),
            inputbuffer(fd_)
        {
            outputbuffer.setHandleCloseCallBack(std::bind(&TcpConnection::handleClose,this));
            inputbuffer.setHandleCloseCallBack(std::bind(&TcpConnection::handleClose,this));
        };

//if inputbuffer over highwater,handle it and put it in outputbuffer,otherwize,update inputbuffer and outputbuffer
        void TcpConnection::handleRead()
        {//处理读事件   
            if(readcallback)
                readcallback(&inputbuffer,&outputbuffer);
            else
                perror("没有注册readcallback\n");
        }

        void TcpConnection::handleWrite()
        {//处理写事件
            if(writecallback)
                writecallback(&inputbuffer,&outputbuffer);
            else
                perror("没有注册writecallback");
        }

        bool TcpConnection::handleWriteForTcpServer()
        {//将buffer中的数据发出去，然后判断是否还有数据
            outputbuffer.writeInSocket();
            return outputbuffer.getDataSize() == 0;
        }
    
        bool TcpConnection::handleReadForTcpClient()
        {//将接收到的数据放到缓冲区中，然后判断是否是一条完整的数据
            inputbuffer.readInSocket();
            return inputbuffer.getKey() == 0;
        }
        
        void TcpConnection::handleClose()
        {   
            if(handlediedtcpconnection)
            {
                handlediedtcpconnection(confd.getFd());
            }
            else
                perror("没有注册handlediedtcpconnection\n");

        }

        void TcpConnection::handleChannel()
        {
            if(wheetchannel)
                wheetchannel();
            else
                perror("没有注册wheetchannel\n");
        }
        
        void TcpConnection::handleDrived()
        {
            if(drivedcallback)
            {
                drivedcallback(&inputbuffer,&outputbuffer);
            }
            else
                perror("没有注册drivedcallback\n");
        }
    }
}





