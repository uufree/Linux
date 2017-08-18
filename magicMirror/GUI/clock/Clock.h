/*************************************************************************
	> File Name: Clock.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月03日 星期六 12时40分22秒
 ************************************************************************/

#ifndef _CLOCK_H
#define _CLOCK_H

#include<string>
#include<vector>
#include<utility>

#include"../../unetComponent/Mutex.h"

namespace utime
{
    class UClock final
    {
        friend bool operator==(const UClock& lhs,const UClock& rhs);
        
        public:
            explicit UClock();
            UClock(const UClock& lhs);
            UClock(UClock&& lhs);
            UClock& operator=(const UClock& lhs);
            UClock& operator=(UClock&& lhs);
            ~UClock();
            
            void swap(UClock& lhs);

            const std::string& getHourAndMinutes() const
            {return hourAndMinutes;};

            const std::string& getMouthAndDays() const
            {return mouthAndDays;};
        
            void update();
        
        private:
            void split(const std::string& lhs,const std::string& rhs,std::vector<std::string>& vecStr);

        private:
            std::string hourAndMinutes;
            std::string mouthAndDays;
            std::vector<std::string> strList;
    };

}


























#endif
