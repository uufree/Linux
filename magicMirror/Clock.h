/*************************************************************************
	> File Name: Clock.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月03日 星期六 12时40分22秒
 ************************************************************************/

#ifndef _CLOCK_H
#define _CLOCK_H

#include<string>

namespace utime
{
    class Clock final
    {
        friend Clock& operator+(const Clock& lhs,const Clock& rhs);
        friend Clock& operator-(const Clock& lhs,const Clock& rhs);
        friend bool operator==(const Clock& lhs,const Clock& rhs);
        
        public:
            explicit Clock();
            explicit Clock(const Clock& lhs);
            explicit Clock(Clock&& lhs);
            Clock& operator=(const Clock& lhs);
            Clock& operator=(Clock&& lhs);
            ~Clock();
            
            void swap();

            const std::string& getHourAndMinutes() const;
            const std::string& getMouthAndDays() const;
            const std::string& getWeek() const;
            const std::string& getYear() const;
        
        private:
            void update();

        private:
            std::string hourAndMinutes;
            std::string mouthAndDays;
            std::string week;
            std::string year;
    };
}


























#endif
