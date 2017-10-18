/*************************************************************************
	> File Name: Clock.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年06月03日 星期六 12时40分28秒
 ************************************************************************/

#include"Clock.h"
#include<time.h>
#include<iostream>

namespace utime
{
    void UClock::split(const std::string& lhs,const std::string& rhs,std::vector<std::string>& vecStr)
    {
        vecStr.clear();
        size_t last = 0;
        size_t index = lhs.find_first_of(rhs,last);
        while(index != std::string::npos)
        {
            vecStr.push_back(lhs.substr(last,index-last));
            last = index + 1;
            index = lhs.find_first_of(rhs,last);
        }

        if((index-last) > 0)
            vecStr.push_back(lhs.substr(last,index-last));
    }
    
    UClock::UClock() 
    {};

    UClock::UClock(const UClock& lhs) : hourAndMinutes(lhs.hourAndMinutes),
        mouthAndDays(lhs.mouthAndDays)
    {};
    
    UClock::UClock(UClock&& lhs) : hourAndMinutes(std::move(lhs.hourAndMinutes)),
    mouthAndDays(std::move(lhs.mouthAndDays))
    {};

    UClock& UClock::operator=(const UClock& lhs)
    {
        if(lhs == *this)
            return *this;
        
        hourAndMinutes = lhs.hourAndMinutes;
        mouthAndDays = lhs.mouthAndDays;
        
        return *this;
    }

    UClock& UClock::operator=(UClock&& lhs)
    {
        hourAndMinutes = std::move(lhs.hourAndMinutes);
        mouthAndDays = std::move(lhs.mouthAndDays);

        return *this;
    }

    UClock::~UClock()
    {};

    void UClock::update()
    {
        time_t time_ = time(NULL);
        std::string strTime = ctime(&time_);
        
        split(strTime," ",strList);
       
        hourAndMinutes = strList[3];
        mouthAndDays = strList[0] + "," + strList[1] + " " + strList[2] + "th";
    };
    
    void UClock::swap(UClock& lhs)
    {
        std::swap(hourAndMinutes,lhs.hourAndMinutes);
        std::swap(mouthAndDays,lhs.mouthAndDays);
        std::swap(strList,lhs.strList);
    }

    bool operator==(const UClock& lhs,const UClock& rhs)
    {
        if(lhs.hourAndMinutes == rhs.hourAndMinutes)
            return true;
        return false;
    }
}

