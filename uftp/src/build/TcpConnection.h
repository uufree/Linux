/*************************************************************************
	> File Name: TcpConnection.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月12日 星期日 09时50分23秒
 ************************************************************************/

#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include"Typedef.h"
#include"Buffer.h"
#include"Socket.h"

namespace unet
{
    namespace net
    {  
        class TcpConnection final
        {
            typedef std::function<void (Buffer* inputbuffer_,Buffer* outputbuffer_)> MessageCallBack;
            typedef std::function<void(int)> HandleDiedTcpConnection;
            typedef std::function<void()> WheetChannelCallBack;

            public:        
                explicit TcpConnection(int fd_);
                TcpConnection(const TcpConnection& lhs) = delete;
                TcpConnection& operator=(const TcpConnection& lhs) = delete;
                ~TcpConnection() {};
//public interface
                void setReadCallBack(const MessageCallBack& cb)
                {readcallback = cb;};//由TcpServer注册

                void setWriteCallBack(const MessageCallBack& cb)
                {writecallback = cb;};//由TcpServer注册

                void setDrivedCallBack(const MessageCallBack& cb)
                {drivedcallback = cb;};//由TcpServer注册，用于处理TcpServer的主动事件

                void setHandleDiedTcpConnection(const HandleDiedTcpConnection& cb)
                {handlediedtcpconnection = cb;};//由TcpServer注册，用于处理销毁TcpConnection的销毁事件
                
                void setWheetChannelCallBack(const WheetChannelCallBack& cb)//不关注所有事件的回调
                {wheetchannel = cb;};
                
                
                bool handleWriteForTcpServer();

                bool handleReadForTcpClient();

                void handleChannel();//不关注所有事件的回调
                
                int asyncRead()
                {
                    return inputbuffer.readInSocket();
                }

                int getFd()//返回自己关注的描述符
                {return confd.getFd();};
                
                Buffer* getOutputBuffer()
                {return &outputbuffer;};

                void handleRead();//用于处理描述符上发生的事件
                void handleWrite();
                void handleClose(); 
                void handleDrived();
                void handleAsyncBuffer();

            private:
                socket::Socket confd;//处理已连接套接字的关闭问题
                Buffer outputbuffer;//消息输出的buffer
                Buffer inputbuffer;//消息输入的buffer
                MessageCallBack readcallback, writecallback,drivedcallback;
                HandleDiedTcpConnection handlediedtcpconnection;
                WheetChannelCallBack wheetchannel;
        };
    }
}
                

                    







































#endif

