/*************************************************************************
	> File Name: EventLoop.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月13日 星期二 19时05分58秒
 ************************************************************************/

/* 设计思路：仅仅是处理事件的容器。
 * 1.使用回调保证最大的灵活性，分离事件与容器的关系。
 * 2.只向外开放两个使用接口以及两个初始化接口。 
 */

/* 核心函数功能：
 * 1.GetActiveEventsCallBack:使用Epoller过滤事件，获得新的发生的事件及其类型，填充ActiveEventsList
 * 2.HandleEventsCallBack:由使用者决定处理事件的方式（同步or异步）
 */

#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include<functional>
#include<vector>
#include<utility>

namespace unet
{
    namespace net
    {
        class EventLoop final
        {
            typedef std::function<void()> GetActiveChannelsCallBack;

            public:
                EventLoop();
                EventLoop(const EventLoop&) = delete;
                EventLoop(EventLoop&& lhs);
                EventLoop& operator=(const EventLoop& lhs) = delete;
                EventLoop& operator=(EventLoop&& lhs);
                ~EventLoop();

                void swap(EventLoop& lhs) = delete;
                
                void loop();
                
                void setQuit()
                {quit = true;};
                
                void setGetActiveChannelsCallBack(const GetActiveChannelsCallBack& lhs)
                {getActiveChannelsCallBack = lhs;};
            
            private:
                bool looping;
                bool quit;
                bool eventHandling;
        
                GetActiveChannelsCallBack getActiveChannelsCallBack;
        };
    }
}

#endif
