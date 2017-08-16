/*************************************************************************
	> File Name: Timestamp.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年07月27日 星期四 13时34分14秒
 ************************************************************************/

#include"Timestamp.h"

namespace unet
{
    namespace time
    {
        Timestamp::Timestamp()
        {
            struct timeval tv;
            ::gettimeofday(&tv,NULL);
            microseconds = tv.tv_sec*KMicroseconds + tv.tv_usec;

        }

        Timestamp::Timestamp(int64_t microseconds_) :
            microseconds(microseconds_)
        {};

        Timestamp::Timestamp(const Timestamp& lhs) :
            microseconds(lhs.microseconds)
        {};

        Timestamp::Timestamp(Timestamp&& lhs) :
            microseconds(std::move(lhs.microseconds))
        {};

        Timestamp& Timestamp::operator=(const Timestamp& lhs)
        {
            microseconds = lhs.microseconds;
            return *this;
        }

        Timestamp& Timestamp::operator=(Timestamp&& lhs)
        {
            microseconds = std::move(lhs.microseconds);
            return *this;
        }
        
        Timestamp::~Timestamp()
        {};

        bool operator<(const Timestamp& lhs,const Timestamp& rhs)
        {return lhs.microseconds < rhs.microseconds;};

        bool operator==(const Timestamp& lhs,const Timestamp& rhs)
        {return lhs.microseconds == rhs.microseconds;};
        
        double timedifference(const Timestamp& lhs,const Timestamp& rhs)
        {
            int64_t diff = lhs.getTime() - rhs.getTime();
            return static_cast<double>(diff/1000000);
        }

    }
}
