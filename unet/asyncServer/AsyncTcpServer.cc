/*************************************************************************
	> File Name: AsyncTcpServer.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月17日 星期一 16时27分23秒
 ************************************************************************/

#include"AsyncTcpServer.h"

namespace unet
{
    namespace net
    {
        AsyncTcpServer::AsyncTcpServer(socket::InetAddress& addr_,int size):
            serveraddr(addr_),
            tcpconnectionMap(),
            channelMap(),
            eventMap(),
            channelList(),
            pool(size),
            epoller(),
            eventLoop(),
            asyncAcceptor(serveraddr),
            timerQueue()
        {
            eventLoop.setGetActiveChannelsCallBack(std::bind(&AsyncTcpServer::GetActiveChannels,this));
            asyncAcceptor.setEraseChannelCallBack(std::bind(&AsyncTcpServer::EraseChannel,this,std::placeholders::_1));
            asyncAcceptor.setInsertChannelCallBack(std::bind(&AsyncTcpServer::InsertChannel,this,std::placeholders::_1));
            
            timerQueue.setInsertChannelCallBack(std::bind(&AsyncTcpServer::InsertChannel,this,std::placeholders::_1));
            timerQueue.setEraseChannelCallBack(std::bind(&AsyncTcpServer::EraseChannel,this,std::placeholders::_1));
        }
        
        AsyncTcpServer::AsyncTcpServer(AsyncTcpServer&& lhs) :
            serveraddr(lhs.serveraddr),
            tcpconnectionMap(std::move(lhs.tcpconnectionMap)),
            channelMap(std::move(lhs.channelMap)),
            eventMap(std::move(lhs.eventMap)),
            channelList(std::move(lhs.channelList)),
            pool(std::move(lhs.pool)),
            epoller(std::move(lhs.epoller)),
            eventLoop(std::move(lhs.eventLoop)),
            asyncAcceptor(std::move(lhs.asyncAcceptor)),
            readCallBack(std::move(lhs.readCallBack))
        {};

        AsyncTcpServer& AsyncTcpServer::operator=(AsyncTcpServer&& lhs)
        {
            tcpconnectionMap = std::move(lhs.tcpconnectionMap);
            channelMap = std::move(lhs.channelMap);
            eventMap = std::move(lhs.eventMap);
            channelList = std::move(lhs.channelList);
            pool = std::move(lhs.pool);
            epoller = std::move(lhs.epoller);
            eventLoop = std::move(lhs.eventLoop);
            asyncAcceptor = std::move(lhs.asyncAcceptor);
            readCallBack = std::move(lhs.readCallBack);
            
            return *this;
        }

        AsyncTcpServer::~AsyncTcpServer()
        {};

        void AsyncTcpServer::InsertChannel(ChannelPtr&& channel)
        {
            channel->setCloseCallBack(std::bind(&AsyncTcpServer::EraseChannel,this,std::placeholders::_1));
            
            TcpConnectionPtr connection(new TcpConnection(channel->getFd()));
            connection->setReadCallBack(readCallBack);
            connection->setCloseCallBack(std::bind(&AsyncTcpServer::EraseChannel,this,std::placeholders::_1));
            channel->setTcpConnectionPtr(connection);

            tcpconnectionMap.insert(connection);
            channelMap.insert(std::move(channel));
            eventMap.insert(channel->getFd(),channel->getEvent(),epoller.getEpollfd());
        }

        void AsyncTcpServer::EraseChannel(int fd)
        {
            eventMap.erase(fd,epoller.getEpollfd());
            tcpconnectionMap.erase(fd);
            channelMap.erase(fd);
        }

        void AsyncTcpServer::GetActiveChannels()
        {
            channelList.clear();
            epoller.epoll(channelList,channelMap);
            if(!channelList.empty())
                pool.addInTaskQueue(channelList); 
        }

        void AsyncTcpServer::start()
        {
            asyncAcceptor.listen();
            timerQueue.start();
            pool.start();
            eventLoop.loop();
        }

        void AsyncTcpServer::addTimer(time::TimerPtr&& timer)
        {
            timerQueue.addTimer(std::move(timer));
        }
    }
}
