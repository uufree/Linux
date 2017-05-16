// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include <muduo/net/Poller.h>

#include <muduo/net/Channel.h>

using namespace muduo;
using namespace muduo::net;

Poller::Poller(EventLoop* loop)
  : ownerLoop_(loop)
{
}

Poller::~Poller()
{
}

bool Poller::hasChannel(Channel* channel) const
{//判断poll中是否有这个channel
  assertInLoopThread();//如果不在loop线程直接退出
  ChannelMap::const_iterator it = channels_.find(channel->fd());
  return it != channels_.end() && it->second == channel;
}

