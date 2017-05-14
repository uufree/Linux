/*************************************************************************
	> File Name: pthread_barrier.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月19日 星期一 01时15分39秒
 ************************************************************************/

#include"apue.h"
#include<pthread.h>
#include<limits.h>
#include<sys/time.h>
#include<iostream>
using namespace std;

#define NTHR 8
#define NUMBER 8000000L
#define TNUM (NUMBER/NTHR)

pthread_barrier_t b;

long nums[NUMBER];
long sums[NUMBER];

int complong(const void* arg1,const void* arg2)
{
    long l1 = *(long*)arg1;
    long l2 = *(long*)arg2;

    if(l1 == l2)
        return 0;
    else if(l1 < l2)
        return -1;
    else 
        return 1;
}

void thr_fn(void* arg)
{
    long idx = (long)arg;
    heapsort(&nums[idx],TNUM,sizeof(long),complong);
    pthread_barrier_wait(&b);
    return (void*)0;
}

void merge()
{
    long idx[NTHR];
    long minidx,num;

    for(int i=0;i<NTHR;i++)
        idx[i] = i * TNUM;
    for(int sidx=0;sidx<NUMBER;sidx++)
    {
        num = LONG_MAX;
        for(int i=0;i<NTHR;i++)
        {
            if((idx[i] < (i+1)*TNUM) && (nums[idx[i]] < num))
            {
                num = nums[idx[i]];
                minidx = i;
            }
        }
        snums[sidx] = nums[idx[minidx]];
        idx[minidx]++;
    }
}

int main()
{
    unsigned long i;
    pthread_t tid;

    srandom(1);
    for(int i=0;i<NUMBER;i++)
        nums[i] = random();

    gettimeofday(&start,NULL);
    pthread_barrier_init(&b,NULL,NTHR+1);
    for(int i=0;i<NTHR;i++)
    {
        err = pthread_create(&tid,NULL,thr_fn,(void*)(i*TNUM));
        if(err != 0)
            cerr << "can't create thread"  << endl;
    }
    pthread_barrier_wait(&b);
    merge();
    gettimeofday(&end,NULL);

    exit(0);
}































