/*************************************************************************
	> File Name: Channel.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月09日 星期四 20时56分25秒
 ************************************************************************/
#include"Channel.h"

namespace unet
{
    namespace net
    {
        const int Channel::KNoneEvent = 0;//关注的事件的处理方式
        const int Channel::KReadEvent = POLLIN;
        const int Channel::KWriteEvent = POLLOUT;
        
        Channel::Channel(int fd_,ChannelType type_) : 
        fd(fd_),
        event(0),
        revent(0),
        handleeventing(false),
        tcpconnectionptr(new TcpConnection(fd_)),
        tcpconnectionwptr(tcpconnectionptr),
        type(type_)
        {
            tcpconnectionptr->setWheetChannelCallBack(std::bind(&Channel::disableAll,this));

            if(type == LISTEN || type == CLOCK)//如果是listenchannel,将ptr reset掉
            {
                tcpconnectionptr.reset();
                tcpconnectionwptr.reset();   
            }
            type = type_;
        };

        Channel::~Channel()
        {
        }

        void Channel::handleEvent()
        {
            if(type == CONNECT)
            {//处理有TcpConnectionPtr的情况
                handleeventing = true;
                if((revent & POLLHUP) || (revent & POLLRDHUP) || (revent & POLLERR))
                {
                    handleClose();
                }
                else if(revent & POLLIN)
                {
                    TcpConnectionPtr wptr = tcpconnectionwptr.lock();
                    if(wptr)
                    {
                        wptr->handleRead();
                    }
                    else
                        handleClose();
                }
                else if(revent & POLLOUT)
                {
                    TcpConnectionPtr wptr = tcpconnectionwptr.lock();
                    if(wptr)
                        wptr->handleWrite();
                    else
                        handleClose();
                }
                else
                    handleClose();

                revent = 0;
                handleeventing = false;
            }
            else if(type == LISTEN)
            {
                handleeventing = true;
                if((revent & POLLHUP) || (revent & POLLRDHUP) || (revent & POLLERR))
                {
                    handleClose();
                }
                else if(revent & POLLIN)
                {
                    if(readcallback)
                        readcallback();
                }
                else
                    handleClose();
                handleeventing = false;
            }
            else if(type == CLOCK)
            {
                handleeventing = true;
                if((revent & POLLHUP) || (revent & POLLRDHUP) || (revent & POLLERR))
                    handleClose();
                else if(revent & POLLIN)
                {
                    if(readcallback)
                        readcallback();
                }
                else
                    handleClose();
            }
            else
            {
                perror("此Channel没有注册类型");
            }
        }
        
        void Channel::handleClose()
        {//处理正常关闭的情况
            if(type == CONNECT)
            {   
                TcpConnectionPtr conptr(tcpconnectionwptr.lock());
                if(conptr)
                {
                    conptr->handleClose();
                }
            }
            disableAll();//这个函数里面会有一个更新的操作
        }
    }
}





        
            
