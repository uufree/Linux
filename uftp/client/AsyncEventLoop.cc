/*************************************************************************
	> File Name: AsyncEventLoop.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月18日 星期二 21时00分21秒
 ************************************************************************/


#include"AsyncEventLoop.h"

namespace unet 
{
    namespace net
    {
        AsyncEventLoop::AsyncEventLoop() : looping(false),quit(false),eventhandling(false)
        {};

        void AsyncEventLoop::loop()
        {
            std::cout << "I'm in looping!" << std::endl;
            std::cout << "----------------------" << std::endl;
            looping = true;
            quit = false;
            while(!quit)
            {
                if(handleasyncbuffer)
                    handleasyncbuffer();

                activechannels.clear();
                    
                if(activecallback)
                    activecallback(&activechannels);
                
                if(!activechannels.empty())
                {
                    eventhandling = true;
                    for(ChannelList::iterator iter=activechannels.begin();iter!=activechannels.end();++iter)
                    {
                        (*iter)->handleEvent();
                    }
                    eventhandling = false;
                }
            }
            looping = false;
        }

        void AsyncEventLoop::setQuit()
        {
            quit = true;
        };

        typedef std::vector<Channel*> ChannelList;//WTF
        ChannelList* AsyncEventLoop::getChannelList()
        {
            activechannels.clear();
            return &activechannels;
        }

    }
}
        
        

