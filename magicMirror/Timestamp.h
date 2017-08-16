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

namespace unet
{
    namespace time
    {    
        static const int KMicroseconds = 1000000;
        
        class Timestamp final
        {
            friend bool operator<(const Timestamp& lhs,const Timestamp& rhs);
            friend bool operator==(const Timestamp& lhs,const Timestamp& rhs);

            public:
                explicit Timestamp();
                explicit Timestamp(int64_t microseconds_);
                Timestamp(const Timestamp& lhs);
                Timestamp(Timestamp&& lhs);
                Timestamp& operator=(const Timestamp& lhs);
                Timestamp& operator=(Timestamp&& lhs);
                ~Timestamp();

                bool operator<(const Timestamp& rhs)
                {return microseconds < rhs.microseconds;};
                
                void addTime(double seconds)
                {microseconds += seconds * KMicroseconds;}

                bool operator==(const Timestamp& rhs)
                {return microseconds == rhs.microseconds;}
        
                void swap(Timestamp& rhs)
                {std::swap(microseconds,rhs.microseconds);};
                    
                int64_t getTime() const
                {return microseconds;}
            
            private:
                int64_t microseconds;
        };
        
        bool operator<(const Timestamp& lhs,const Timestamp& rhs);
        bool operator==(const Timestamp& lhs,const Timestamp& rhs);
        
        double timedifference(const Timestamp& lhs,const Timestamp& rhs);
    }
}

#endif

