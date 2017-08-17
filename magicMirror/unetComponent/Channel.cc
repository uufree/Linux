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
        
        Channel::Channel(int fd_,ChannelType type_) : 
            fd(fd_),
            event(EPOLLIN|EPOLLOUT),
            revent(0),
            handleEventing(false),
            type(type_)
        {};

        Channel::Channel(Channel&& lhs) : 
            fd(lhs.fd),
            event(lhs.event),
            revent(lhs.revent),
            handleEventing(false),
            type(lhs.type)
        {};

        Channel& Channel::operator=(Channel&& lhs)
        {
            fd = lhs.fd;
            event = lhs.event;
            revent = lhs.revent;
            handleEventing = lhs.handleEventing;
            type = lhs.type;

            return *this;
        }

        Channel::~Channel()
        {};

        void Channel::handleEvent()
        {
            if(type == CLOCK)
            {
                handleEventing = true;
                if((revent & EPOLLHUP) || (revent & EPOLLRDHUP) || (revent & EPOLLERR))
                {
                    if(closeCallBack)
                        closeCallBack(fd);
                    else
                        perror("没有注册CloseCallBack\n");
                }
                else if(revent & EPOLLIN)
                {
                    if(readCallBack)
                        readCallBack();
                    else
                        perror("没有注册ReadCallBack\n");
                }
                else
                {
                    if(closeCallBack)
                        closeCallBack(fd);
                    else
                        perror("没有注册CloseCallBack\n");
                }

                revent = 0;
                handleEventing = false;
            }
            else
            {
                perror("此Channel没有注册类型");
            }
        }
    }
}
