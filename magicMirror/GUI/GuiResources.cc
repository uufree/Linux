/*************************************************************************
	> File Name: GuiResources.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年08月17日 星期四 22时19分58秒
 ************************************************************************/

#include"GuiResources.h"

GuiResources::GuiResources() :
    lock(),
    clock()
{};

GuiResources::GuiResources(GuiResources&& lhs) :
    lock(std::move(lhs.lock)),
    clock(std::move(lhs.clock))
{};

GuiResources& GuiResources::operator=(GuiResources&& lhs)
{
    clock = std::move(lhs.clock);
    hourAndMinutes = std::move(lhs.hourAndMinutes);
    mouthAndDays = std::move(lhs.mouthAndDays);

    return *this;
}

GuiResources::~GuiResources()
{};

void GuiResources::ClockUpdate()
{
    clock.update();
    
    {
        unet::thread::MutexLockGuard guard(lock);
        hourAndMinutes = clock.getHourAndMinutes();
        mouthAndDays = clock.getMouthAndDays();
    }
}
