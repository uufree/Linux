// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include <muduo/net/TcpConnection.h>

#include <muduo/base/Logging.h>
#include <muduo/base/WeakCallback.h>
#include <muduo/net/Channel.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/Socket.h>
#include <muduo/net/SocketsOps.h>

#include <boost/bind.hpp>

#include <errno.h>

using namespace muduo;
using namespace muduo::net;

void muduo::net::defaultConnectionCallback(const TcpConnectionPtr& conn)
{//muduo提供的默认的连接回调函数，就是做日志
  LOG_TRACE << conn->localAddress().toIpPort() << " -> "
            << conn->peerAddress().toIpPort() << " is "
            << (conn->connected() ? "UP" : "DOWN");
  // do not call conn->forceClose(), because some users want to register message callback only.
}

void muduo::net::defaultMessageCallback(const TcpConnectionPtr&,
                                        Buffer* buf,
                                        Timestamp)
{//muduo提供的默认的收到信息时的回调函数
  buf->retrieveAll();
}

TcpConnection::TcpConnection(EventLoop* loop,
                             const string& nameArg,
                             int sockfd,
                             const InetAddress& localAddr,
                             const InetAddress& peerAddr)
  : loop_(CHECK_NOTNULL(loop)),
    name_(nameArg),
    state_(kConnecting),
    socket_(new Socket(sockfd)),
    channel_(new Channel(loop, sockfd)),
    localAddr_(localAddr),
    peerAddr_(peerAddr),
    highWaterMark_(64*1024*1024),
    reading_(true)
{
  channel_->setReadCallback(
      boost::bind(&TcpConnection::handleRead, this, _1));
  channel_->setWriteCallback(
      boost::bind(&TcpConnection::handleWrite, this));
  channel_->setCloseCallback(
      boost::bind(&TcpConnection::handleClose, this));
  channel_->setErrorCallback(
      boost::bind(&TcpConnection::handleError, this));
  LOG_DEBUG << "TcpConnection::ctor[" <<  name_ << "] at " << this
            << " fd=" << sockfd;
  socket_->setKeepAlive(true);//用于处理连接不可到达的情况，一般在服务器端进行设置
}

TcpConnection::~TcpConnection()
{
  LOG_DEBUG << "TcpConnection::dtor[" <<  name_ << "] at " << this
            << " fd=" << channel_->fd()
            << " state=" << stateToString();
  assert(state_ == kDisconnected);//判断con是否关闭
}

bool TcpConnection::getTcpInfo(struct tcp_info* tcpi) const
{
  return socket_->getTcpInfo(tcpi);
}

string TcpConnection::getTcpInfoString() const
{
  char buf[1024];
  buf[0] = '\0';
  socket_->getTcpInfoString(buf, sizeof buf);
  return buf;
}

void TcpConnection::send(const void* data, int len)
{//转化成另外一种send方式
  send(StringPiece(static_cast<const char*>(data), len));
}

void TcpConnection::send(const StringPiece& message)
{
  if (state_ == kConnected)//如果对象目前处于连接状态
  {
    if (loop_->isInLoopThread())//如果在IO线程，直接发送，否则封装成函数对象然后发送
    {
      sendInLoop(message);
    }
    else
    {
      loop_->runInLoop(
          boost::bind(&TcpConnection::sendInLoop,
                      this,     // FIXME
                      message.as_string()));//如果没有在loop线程，注册成functor加入loop线程的预处理队列
                    //std::forward<string>(message)));
    }
  }
}

// FIXME efficiency!!!
void TcpConnection::send(Buffer* buf)//同上述的send，不过是使用muduo提供的buffer
{
  if (state_ == kConnected)
  {
    if (loop_->isInLoopThread())
    {
      sendInLoop(buf->peek(), buf->readableBytes());
      buf->retrieveAll();
    }
    else
    {
      loop_->runInLoop(
          boost::bind(&TcpConnection::sendInLoop,
                      this,     // FIXME
                      buf->retrieveAllAsString()));
                    //std::forward<string>(message)));
    }
  }
}

void TcpConnection::sendInLoop(const StringPiece& message)
{//转化成下述的sendInLoop
  sendInLoop(message.data(), message.size());
}

void TcpConnection::sendInLoop(const void* data, size_t len)
{//前面已经断言：如果在loop线程直接调用这个函数
  loop_->assertInLoopThread();//判断是否在loop线程
  ssize_t nwrote = 0;//已经写了0字节
  size_t remaining = len;
  bool faultError = false;
  if (state_ == kDisconnected)//如果连接没有打开直接退出
  {
    LOG_WARN << "disconnected, give up writing";
    return;
  }
  // if no thing in output queue, try writing directly
  if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0)
  {
    nwrote = sockets::write(channel_->fd(), data, len);//写进sockfd
    if (nwrote >= 0)//如果有写入
    {
      remaining = len - nwrote;//获得写入的字节
      if (remaining == 0 && writeCompleteCallback_)//全部写入的情况
      {
        loop_->queueInLoop(boost::bind(writeCompleteCallback_, shared_from_this()));//将完全写入的情况注册到loop线程
      }
    }
    else // nwrote < 0
    {//处理写入错误的情况
      nwrote = 0;
      if (errno != EWOULDBLOCK)
      {
        LOG_SYSERR << "TcpConnection::sendInLoop";
        if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
        {
          faultError = true;
        }
      }
    }
  }

  assert(remaining <= len);//没有完全写入
  if (!faultError && remaining > 0)
  {
    size_t oldLen = outputBuffer_.readableBytes();
    if (oldLen + remaining >= highWaterMark_
        && oldLen < highWaterMark_
        && highWaterMarkCallback_)
    {
      loop_->queueInLoop(boost::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
    }
    outputBuffer_.append(static_cast<const char*>(data)+nwrote, remaining);
    if (!channel_->isWriting())
    {
      channel_->enableWriting();
    }
  }
}

void TcpConnection::shutdown()
{//将关闭写功能加入loop线程的预处理队列
  // FIXME: use compare and swap
  if (state_ == kConnected)
  {
    setState(kDisconnecting);
    // FIXME: shared_from_this()?
    loop_->runInLoop(boost::bind(&TcpConnection::shutdownInLoop, this));//将所有的事情放到IO线程去做，但是没有搞懂this的原因
  }
}

void TcpConnection::shutdownInLoop()
{//关闭sockfd的写功能，保存读
  loop_->assertInLoopThread();
  if (!channel_->isWriting())
  {
    // we are not writing
    socket_->shutdownWrite();
  }
}

// void TcpConnection::shutdownAndForceCloseAfter(double seconds)
// {
//   // FIXME: use compare and swap
//   if (state_ == kConnected)
//   {
//     setState(kDisconnecting);
//     loop_->runInLoop(boost::bind(&TcpConnection::shutdownAndForceCloseInLoop, this, seconds));
//   }
// }

// void TcpConnection::shutdownAndForceCloseInLoop(double seconds)
// {
//   loop_->assertInLoopThread();
//   if (!channel_->isWriting())
//   {
//     // we are not writing
//     socket_->shutdownWrite();
//   }
//   loop_->runAfter(
//       seconds,
//       makeWeakCallback(shared_from_this(),
//                        &TcpConnection::forceCloseInLoop));
// }

void TcpConnection::forceClose()
{
  // FIXME: use compare and swap
  if (state_ == kConnected || state_ == kDisconnecting)
  {//注册关闭的回调函数
    setState(kDisconnecting);
    loop_->queueInLoop(boost::bind(&TcpConnection::forceCloseInLoop, shared_from_this()));
  }
}

void TcpConnection::forceCloseWithDelay(double seconds)
{//在一段时间后关闭
  if (state_ == kConnected || state_ == kDisconnecting)
  {
    setState(kDisconnecting);
    loop_->runAfter(
        seconds,
        makeWeakCallback(shared_from_this(),
                         &TcpConnection::forceClose));  // not forceCloseInLoop to avoid race condition
  }
}

void TcpConnection::forceCloseInLoop()
{//处理关闭的情况，这样写貌似优点复杂了，类内部的函数调用有点复杂了
  loop_->assertInLoopThread();//断言是否在loop线程
  if (state_ == kConnected || state_ == kDisconnecting)
  {
    // as if we received 0 byte in handleRead();
    handleClose();
  }
}

const char* TcpConnection::stateToString() const
{//获得目前con的状态
  switch (state_)
  {
    case kDisconnected:
      return "kDisconnected";
    case kConnecting:
      return "kConnecting";
    case kConnected:
      return "kConnected";
    case kDisconnecting:
      return "kDisconnecting";
    default:
      return "unknown state";
  }
}

void TcpConnection::setTcpNoDelay(bool on)
{//开启低延迟算法
  socket_->setTcpNoDelay(on);
}

void TcpConnection::startRead()
{//在loop线程中将本con设置为可读的
  loop_->runInLoop(boost::bind(&TcpConnection::startReadInLoop, this));
}

void TcpConnection::startReadInLoop()
{
  loop_->assertInLoopThread();//断言是否在loop线程
  if (!reading_ || !channel_->isReading())
  {
    channel_->enableReading();//设置channel的可读属性
    reading_ = true;
  }
}

void TcpConnection::stopRead()//通过调整channel，关闭con的可读属性
{
  loop_->runInLoop(boost::bind(&TcpConnection::stopReadInLoop, this));
}

void TcpConnection::stopReadInLoop()
{
  loop_->assertInLoopThread();
  if (reading_ || channel_->isReading())
  {
    channel_->disableReading();
    reading_ = false;
  } 
}

void TcpConnection::connectEstablished()
{//建立连接
  loop_->assertInLoopThread();//断言是否在IO线程
  assert(state_ == kConnecting);//正处于连接建立过程
  setState(kConnected);
  channel_->tie(shared_from_this());//使channel的tie的指向不为空
  channel_->enableReading();//将connection设置为可读的

  connectionCallback_(shared_from_this());//用户提供的回调函数，muduo有提供默认的
}

void TcpConnection::connectDestroyed()
{//销毁连接
  loop_->assertInLoopThread();//断言是否在loop线程
  if (state_ == kConnected)//如果此时处于连接状态
  {
    setState(kDisconnected);//将状态设置为不可连接状态
    channel_->disableAll();//channel不再关注任何事件

    connectionCallback_(shared_from_this());//记录作用，好坑的一个作用
  }
  channel_->remove();//在poller中移除channel
}

void TcpConnection::handleRead(Timestamp receiveTime)
{//都是向channel注册的函数
  loop_->assertInLoopThread();//断言在loop线程
  int savedErrno = 0;//在读取数据之后调用用户提供的回调函数
  ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
  if (n > 0)
  {//这个应该时用户提供的处理信息的回调函数
    messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
  }
  else if (n == 0)
  {//读到了0,直接关闭
    handleClose();
  }
  else
  {//如果有错误
    errno = savedErrno;
    LOG_SYSERR << "TcpConnection::handleRead";
    handleError();//处理关闭
  }
}

void TcpConnection::handleWrite()
{//向channel注册的写回调函数
  loop_->assertInLoopThread();//断言是否在Loop线程
  if (channel_->isWriting())//判断是否可写
  {
    ssize_t n = sockets::write(channel_->fd(),
                               outputBuffer_.peek(),
                               outputBuffer_.readableBytes());
    if (n > 0)//写了一部分
    {
      outputBuffer_.retrieve(n);
      if (outputBuffer_.readableBytes() == 0)
      {//如果writebuffer没有数据了
        channel_->disableWriting();//关闭channel的写事件
        if (writeCompleteCallback_)
        {//在loop中处理全部写完的事情
          loop_->queueInLoop(boost::bind(writeCompleteCallback_, shared_from_this()));
        }
        if (state_ == kDisconnecting)//如果con正在关闭
        {
          shutdownInLoop();//关闭con的write功能
        }
      }
    }
    else//出现写错误
    {
      LOG_SYSERR << "TcpConnection::handleWrite";
      // if (state_ == kDisconnecting)
      // {
      //   shutdownInLoop();
      // }
    }
  }
  else//如果不可写
  {
    LOG_TRACE << "Connection fd = " << channel_->fd()
              << " is down, no more writing";
  }
}

void TcpConnection::handleClose()
{//处理关闭事件
  loop_->assertInLoopThread();//断言是否在loop线程
  LOG_TRACE << "fd = " << channel_->fd() << " state = " << stateToString();
  assert(state_ == kConnected || state_ == kDisconnecting);
  // we don't close fd, leave it to dtor, so we can find leaks easily.
  setState(kDisconnected);//设置关闭状态
  channel_->disableAll();//不再关注任何事情

  TcpConnectionPtr guardThis(shared_from_this());//获得shared_ptr交由tcpsever处理
  connectionCallback_(guardThis);//这他妈就是记录一点日志
  // must be the last line
  closeCallback_(guardThis);
}

void TcpConnection::handleError()
{//处理错误事件
  int err = sockets::getSocketError(channel_->fd());
  LOG_ERROR << "TcpConnection::handleError [" << name_
            << "] - SO_ERROR = " << err << " " << strerror_tl(err);
}

