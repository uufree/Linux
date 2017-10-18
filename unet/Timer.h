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

namespace unet
{
    namespace time
    {
        typedef std::function<void()> TimeCallBack;
        
        class Timer final
        {
            friend bool operator==(const Timer& lhs,const Timer& rhs);
            friend bool operator<(const Timer& lhs,const Timer& rhs);

            public:
                explicit Timer(Timestamp time_,bool repeat_,double repeatTime_);
                explicit Timer(bool repeat_,double repeatTime_);
                Timer(const Timer& lhs) = delete;
                Timer(Timer&& lhs);
                Timer& operator=(const Timer& lhs) = delete;
                Timer& operator=(Timer&& lhs);
                ~Timer();
                
                void run() const
                {timeCallBack();};

                bool isRepeat() const
                {return repeat;};
                
                void setTimeCallBack(const TimeCallBack& lhs)
                {timeCallBack = lhs;};
                
                double getRepeatTime()
                {return repeatTime;};

            private:
                Timestamp time;
                bool repeat;
                double repeatTime;
                TimeCallBack timeCallBack;
        };

        bool operator<(const Timer& lhs,const Timer& rhs);
        bool operator==(const Timer& lhs,const Timer& rhs);
    }
}

#endif

