/*************************************************************************
	> File Name: Timestamp.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年02月27日 星期一 16时17分32秒
 ************************************************************************/

#ifndef _TIMESTAMP_H
#define _TIMESTAMP_H

#include<time.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdint.h>
#include<iostream>

//将时间封装为微秒级的长整数

namespace unet
{
    namespace time
    {
        class Timestamp final
        {
            
            public:
                explicit Timestamp()
                {
                    struct timeval tv;
                    ::gettimeofday(&tv,NULL);
                    microseconds = tv.tv_sec*KMicroseconds + tv.tv_usec;
                }

                Timestamp(int64_t microseconds_) : 
                    microseconds(microseconds_)
                {};

                friend bool operator<(const Timestamp& lhs,Timestamp& rhs);
                
                Timestamp(Timestamp& rhs) :
                    microseconds(rhs.microseconds)
                {};
                

                void addTime(double seconds)
                {
                    microseconds += seconds * KMicroseconds;
                }
/*                
                bool operator<(const Timestamp& lhs)
                {
                    return microseconds<lhs.microseconds;
                };
*/

                bool operator==(const Timestamp& rhs)
                {
                    return microseconds == rhs.microseconds;
                }
        
                void swap(Timestamp& rhs)
                {
                    std::swap(microseconds,rhs.microseconds);
                };
                    
                int64_t getTime() const
                {
                    return microseconds;
                }
                
            static const int KMicroseconds = 1000000;
            
            private:
                int64_t microseconds;
        };
        
        bool operator<(const Timestamp& lhs,Timestamp& rhs)
        {
            return lhs.microseconds < rhs.microseconds;
        }
        
        inline double timedifference(const Timestamp& lhs,Timestamp& rhs)
        {
            int64_t diff = lhs.getTime() - rhs.getTime();
            return static_cast<double>(diff/Timestamp::KMicroseconds);
        }
    }
}

#endif

