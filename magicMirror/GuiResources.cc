/*************************************************************************
	> File Name: GuiResources.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年08月17日 星期四 22时19分58秒
 ************************************************************************/

#include"GuiResources.h"

GuiResources::GuiResources() :
    system(),
    clock()
{};

GuiResources::GuiResources(GuiResources&& lhs) :
    system(),
    clock(std::move(lhs.clock))
{};

GuiResources& GuiResources::operator=(GuiResources&& lhs)
{
    system = std::move(lhs.system);
    clock = std::move(lhs.clock);

    return *this;
}

GuiResources::~GuiResources()
{
    system.stop();
};

void GuiResources::ClockUpdate()
{
    unet::thread::MutexLockGuard guard(lock);
    clock.update();
}

void GuiResources::start()
{
    {//clock
        unet::time::TimerPtr ptr(new unet::time::Timer(true,1));
        ptr->setTimeCallBack(std::bind(&GuiResources::ClockUpdate,this));
        system.addTimer(std::move(ptr));
    }
}
