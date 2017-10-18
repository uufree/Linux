/*************************************************************************
	> File Name: TaskThreadPool.cc
	> Author: uuchen
	> Mail: 1319081676@qq.com
	> Created Time: 2017年04月17日 星期一 19时47分39秒
 ************************************************************************/

#include"../TaskThreadPool.h"

using namespace unet::thread;

typedef std::function<void()> Task;
typedef std::vector<Task> VecTask;

void func(int num)
{
    printf("---%ld---%d\n",pthread_self(),num);
    sleep(1);
}

int main(int argc,char** argv)
{
    TaskThreadPool pool(2);
    VecTask tasks;
    VecTask tasks1;
    pool.start();
    
    for(int i=0;i<10;++i)
    {
        tasks.push_back(std::bind(&func,i));    
        tasks1.push_back(std::bind(&func,i+10));
    }

    pool.addInTaskQueue(std::move(tasks));
    pool.addInTaskQueue(std::move(tasks1));

    sleep(30);
    return 0;
}
