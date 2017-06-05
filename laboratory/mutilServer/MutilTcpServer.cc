/*************************************************************************
	> File Name: MutilTcpServer.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月29日 星期三 02时31分02秒
 ************************************************************************/

#include"MutilTcpServer.h"

namespace unet
{
    namespace net
    {
        MutilTcpServer::MutilTcpServer(InetAddress* addr_,int size) :
            current(new thread::Current),
            serveraddr(addr_),
            acceptor(new MutilAcceptor(addr_,size))
//            timerqueue(new time::TimerQueue())
        {
            acceptor->setNewConnectionCallBack(std::bind(&MutilTcpServer::newConnectionCallBack,this,std::placeholders::_1));
            acceptor->setAddInServerLoopCallBack(std::bind(&MutilTcpServer::addInServerLoop,this,std::placeholders::_1));
 
//            timerqueue->setAddInServerLoopCallBack(std::bind(&MutilTcpServer::addInServerLoop,this,std::placeholders::_1));
        };
        
        MutilTcpServer::~MutilTcpServer()
        {
            current->quit();
            acceptor->join(); 
        }

        Channel* MutilTcpServer::newConnectionCallBack(int fd_)
        {
            Channel* channel = new Channel(fd_,CONNECT);
            TcpConnectionPtr ptr(channel->getTcpConnectionPtr());

            if(readcallback)
                ptr->setReadCallBack(readcallback);
            if(writecallback)
                ptr->setWriteCallBack(writecallback);
            if(drivedcallback)
                ptr->setDrivedCallBack(drivedcallback);
            
            ptr->setHandleDiedTcpConnection(std::bind(&MutilTcpServer::handleDiedTcpConnection,this,std::placeholders::_1));
            tcpconnectionptrmap.insert({fd_,ptr});
            
            return channel;
        };

        void MutilTcpServer::addInServerLoop(Channel* channel)
        {
            unet::thread::MutexLockGuard guard(mutex);
            current->addInEpoller(channel);
        };
        
        void MutilTcpServer::start()
        {
            acceptor->listen();
            acceptor->startLoop();
            current->startLoop();
        };

        void MutilTcpServer::handleDiedTcpConnection(int fd)
        {
            thread::MutexLockGuard guard(mutex);
            tcpconnectionptrmap[fd]->handleChannel();
            tcpconnectionptrmap[fd].reset();
            tcpconnectionptrmap.erase(fd);
        };
    }
}




