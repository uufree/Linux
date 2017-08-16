/*************************************************************************
	> File Name: Timer.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月27日 星期四 13时56分23秒
 ************************************************************************/

#include"Timer.h"

namespace unet
{
    namespace time
    {
        Timer::Timer(Timestamp time_,bool repeat_,double repeatTime_) :
            time(time_),
            repeat(repeat_),
            repeatTime(repeatTime_)
        {};

        Timer::Timer(bool repeat_,double repeatTime_) :
            time(Timestamp()),
            repeat(repeat_),
            repeatTime(repeatTime_)
        {};

        Timer::Timer(Timer&& lhs) :
            time(std::move(lhs.time)),
            repeat(lhs.repeat),
            repeatTime(lhs.repeatTime),
            timeCallBack(std::move(lhs.timeCallBack))
        {};
          
        Timer& Timer::operator=(Timer&& lhs)
        {
            time = std::move(lhs.time);
            repeat = lhs.repeat;
            repeatTime = lhs.repeatTime;
            timeCallBack = std::move(lhs.timeCallBack);
            
            return *this;
        }
        
        Timer::~Timer()
        {};

        bool operator<(const Timer& lhs,const Timer& rhs)
        {
            return lhs.time < rhs.time;
        }

        bool operator==(const Timer& lhs,const Timer& rhs)
        {
            return lhs.time == rhs.time;
        }
    }
}

