/*************************************************************************
	> File Name: ThreadPool.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年03月27日 星期一 23时31分49秒
 ************************************************************************/

#include"ThreadPool.h"

namespace unet
{
    namespace thread
    {
        ThreadPool::ThreadPool(int size) : 
        threadsize(size)
        {};

        ThreadPool::~ThreadPool()
        {
            for(auto iter=threadmap.begin();iter!=threadmap.end();++iter)
            {
                ::pthread_detach(iter->first);
            }
        }

        void ThreadPool::start()
        {
            for(int i=0;i<threadsize;++i)
            {
                thread.start();
                threadmap.insert({thread.getThreadId(),thread});
            }
        }

        void ThreadPool::joinAll()
        {
            for(auto iter=threadmap.begin();iter!=threadmap.end();++iter)
            {
                ::pthread_join(iter->first,NULL);
                threadmap.erase(iter);
            }
        }
    }
}
