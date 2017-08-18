/*************************************************************************
	> File Name: GuiResources.h
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年08月17日 星期四 22时19分41秒
 ************************************************************************/

#ifndef _GUIRESOURCES_H
#define _GUIRESOURCES_H

#include"./GUI/clock/Clock.h"
#include"./unetComponent/Mutex.h"
#include"./timerSystem/TimerEventSystem.h"

class GuiResources final
{
    public:
        GuiResources();
        GuiResources(const GuiResources& lhs) = delete;
        GuiResources(GuiResources&& lhs);
        GuiResources& operator=(const GuiResources& lhs);
        GuiResources& operator=(GuiResources&& lhs);
        ~GuiResources();
        
        const std::string&  getClock_HourAndMinutes()
        {return clock.getHourAndMinutes();};

        const std::string&  getClock_MouthAndDays()
        {return clock.getMouthAndDays();};
        
        void start();

    private:
        void ClockUpdate();

    private:
        unet::thread::MutexLock lock;
        unet::net::TimerEventSystem system;
        utime::UClock clock;
};

#endif
