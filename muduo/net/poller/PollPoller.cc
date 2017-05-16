// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include <muduo/net/poller/PollPoller.h>

#include <muduo/base/Logging.h>
#include <muduo/base/Types.h>
#include <muduo/net/Channel.h>

#include <assert.h>
#include <errno.h>
#include <poll.h>

using namespace muduo;
using namespace muduo::net;

PollPoller::PollPoller(EventLoop* loop)
  : Poller(loop)
{//一个Loop只能有一个poll
}

PollPoller::~PollPoller()
{
}

Timestamp PollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
  // XXX pollfds_ shouldn't change
  int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);//获得活动的文件描述符数组
  int savedErrno = errno;
  Timestamp now(Timestamp::now());//获得当前时间
  if (numEvents > 0)//如果有活动的文件描述符
  {
    LOG_TRACE << numEvents << " events happended";
    fillActiveChannels(numEvents, activeChannels);//将活动的文件描述符数组对应成channels并且返回活动的channels
  }
  else if (numEvents == 0)
  {
    LOG_TRACE << " nothing happended";
  }
  else
  {
    if (savedErrno != EINTR)
    {
      errno = savedErrno;
      LOG_SYSERR << "PollPoller::poll()";
    }
  }
  return now;
}

void PollPoller::fillActiveChannels(int numEvents,
                                    ChannelList* activeChannels) const
{
  for (PollFdList::const_iterator pfd = pollfds_.begin();
      pfd != pollfds_.end() && numEvents > 0; ++pfd)
  {
    if (pfd->revents > 0)//寻找活动的pfd
    {
      --numEvents;//找到一个，减少数目
      ChannelMap::const_iterator ch = channels_.find(pfd->fd);//获得pair<int,channel>，在map中有合理的记数
      assert(ch != channels_.end());//判断是否到达map的末尾
      Channel* channel = ch->second;//获得channel
      assert(channel->fd() == pfd->fd);//检查fd是否相等
      channel->set_revents(pfd->revents);//将channel当前发生的事件设置为pfd中发生的事件
      // pfd->revents = 0;
      activeChannels->push_back(channel);//将channel加入活动的channels
    }
  }
}

void PollPoller::updateChannel(Channel* channel)
{//将channel关注的事件与pollfd同步
  Poller::assertInLoopThread();//如果不再loop线程直接退出
  LOG_TRACE << "fd = " << channel->fd() << " events = " << channel->events();
  if (channel->index() < 0)//获得channel在map中的位置
  {
    // a new one, add to pollfds_
    assert(channels_.find(channel->fd()) == channels_.end());
    struct pollfd pfd;//新建一个pfd与channel相关联
    pfd.fd = channel->fd();
    pfd.events = static_cast<short>(channel->events());//关注的事件设置为channel关注的事件
    pfd.revents = 0;//正在发生的事件为0
    pollfds_.push_back(pfd);//将设置好的pollfd加入关注事件列表
    int idx = static_cast<int>(pollfds_.size())-1;//并且获得加入的位置
    channel->set_index(idx);//channel保存自己在pollfds中的位置
    channels_[pfd.fd] = channel;//channel将自己加入到channelmap中
  }
  else
  {
    // update existing one
    assert(channels_.find(channel->fd()) != channels_.end());
    assert(channels_[channel->fd()] == channel);//判断位置是否正确
    int idx = channel->index();//获得channel在pollfd中的索引
    assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
    struct pollfd& pfd = pollfds_[idx];//获得索引
    assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd()-1);
    pfd.events = static_cast<short>(channel->events());//修改关注的事件
    pfd.revents = 0;//将当前发生的事件设置为0
    if (channel->isNoneEvent())//如果channel没有任何事件，一个暂时熄火的channel
    {
      // ignore this pollfd
      pfd.fd = -channel->fd()-1;//将索引设置为原来索引的负数
    }
  }
}

void PollPoller::removeChannel(Channel* channel)
{
  Poller::assertInLoopThread();//判断是否在IO线程
  LOG_TRACE << "fd = " << channel->fd();
  assert(channels_.find(channel->fd()) != channels_.end());
  assert(channels_[channel->fd()] == channel);
  assert(channel->isNoneEvent());
  int idx = channel->index();//获得pfd位置的索引
  assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
  const struct pollfd& pfd = pollfds_[idx]; (void)pfd;//获得pfd
  assert(pfd.fd == -channel->fd()-1 && pfd.events == channel->events());
  size_t n = channels_.erase(channel->fd());//在Map中删除channel
  assert(n == 1); (void)n;//准备删除pollfd中的关注事件
  if (implicit_cast<size_t>(idx) == pollfds_.size()-1)//获得pollfd的索引
  {
    pollfds_.pop_back();
  }
  else//想方设法的删除pollfd中与channel相关的pfd
  {
    int channelAtEnd = pollfds_.back().fd;
    iter_swap(pollfds_.begin()+idx, pollfds_.end()-1);
    if (channelAtEnd < 0)
    {
      channelAtEnd = -channelAtEnd-1;
    }
    channels_[channelAtEnd]->set_index(idx);
    pollfds_.pop_back();
  }
}

