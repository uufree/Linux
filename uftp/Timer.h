/*************************************************************************
	> File Name: Timer.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月10日 星期五 21时04分39秒
 ************************************************************************/

#ifndef _TIMER_H
#define _TIMER_H

#include"Timestamp.h"
#include<functional>

//表示一个常数时间所对应的事件

namespace unet
{
    namespace time
    {
        class Timer final
        {
            typedef std::function<void()> TimeCallBack;
            
            public:
                explicit Timer(const TimeCallBack& callback_,bool repeat_,double repeattime_) : callback(callback_),
                repeat(repeat_),repeattime(repeattime_)
                {
                };

                Timer(const Timer& lhs) = delete;
                Timer& operator=(const Timer& lhs) = delete;
                
                ~Timer(){};

                void run()
                {
                    callback();
                }

                bool isRepeat()
                {
                    return repeat;
                }

            private:
                const TimeCallBack callback;
                bool repeat;
                double repeattime;
                Timestamp time;
        };
    }
}

#endif

