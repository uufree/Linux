/*************************************************************************
	> File Name: AsyncAcceptor.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月17日 星期一 21时29分55秒
 ************************************************************************/

#include"AsyncAcceptor.h"
#include"../Socket.h"

namespace unet
{
    namespace net
    {
        AsyncAcceptor::AsyncAcceptor(socket::InetAddress& addr_):
            serveraddr(addr_),
            listenfd(socket::socket()),
            listening(false)
        {
            socket::bind(listenfd,serveraddr);
            socket::listen(listenfd);
        }

        AsyncAcceptor::AsyncAcceptor(AsyncAcceptor&& lhs) :
            serveraddr(lhs.serveraddr),
            listenfd(std::move(lhs.listenfd)),
            listening(false),
            insertChannelCallBack(std::move(lhs.insertChannelCallBack)),
            eraseChannelCallBack(std::move(lhs.eraseChannelCallBack))
        {
            lhs.stopListen();
        };

        AsyncAcceptor& AsyncAcceptor::operator=(AsyncAcceptor&& lhs)
        {
            lhs.stopListen();
            
            serveraddr = std::move(lhs.serveraddr);
            listenfd = std::move(lhs.listenfd);
            listening = false;
            insertChannelCallBack = std::move(lhs.insertChannelCallBack);
            eraseChannelCallBack = std::move(lhs.eraseChannelCallBack);
            
            return *this;
        }

        AsyncAcceptor::~AsyncAcceptor()
        {
            if(!listening)
                eraseChannelCallBack(listenfd.getFd());
        };

        void AsyncAcceptor::listen()
        {
            ChannelPtr channel(new Channel(listenfd.getFd(),LISTEN));
            channel->setReadCallBack(std::bind(&AsyncAcceptor::handleRead,this));
            channel->setCloseCallBack(eraseChannelCallBack);

            if(!listening)
            {
                listening = true;
                
                if(insertChannelCallBack)
                    insertChannelCallBack(std::move(channel));
                else
                    perror("not register InsertChannelCallBack\n");
            }
        }

        void AsyncAcceptor::stopListen()
        {
            if(listening)
            {
                listening = false;

                if(eraseChannelCallBack)
                    eraseChannelCallBack(listenfd.getFd());
                else 
                    perror("not register EraseChannelCallBack\n");
            }
        }

        void AsyncAcceptor::handleRead()
        {
            int confd = socket::accept(listenfd.getFd());
            assert(confd >= 0);
            socket::setNonBlockAndCloseOnExec(confd);
            
            ChannelPtr channel(new Channel(confd,CONNECT));
            channel->setCloseCallBack(eraseChannelCallBack);

            if(insertChannelCallBack)
                insertChannelCallBack(std::move(channel));
            else
                perror("not register InsertChannelCallBack\n");
        }
    }
}

